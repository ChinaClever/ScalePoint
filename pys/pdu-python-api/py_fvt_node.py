#!/usr/bin/python3

import sys, time, csv, socket
from raritan import rpc
from raritan import zeroconf
#from raritan.rpc.pdumodel import *
from raritan.rpc import Agent, pdumodel, firmware, test, usb
from raritan.rpc import servermon, event, usermgmt, um, devsettings, pdumodel, cert, sensors

def devIpAddr():
    ips = zeroconf.discover();
    if(len(ips) > 0): return ips[0]
    else: print('use default IP: 192.168.1.100')
    return '192.168.1.162'

def login():
    try:
        ip = sys.argv[1]
        user = sys.argv[2]
        pw = sys.argv[3]
    except:
        user = "admin"
        pw = "Admin123"
        ip = devIpAddr()

    try:
        agent = Agent("https", ip, user, pw, disable_certificate_verification=True, timeout = 5)
    except:
        print("login err PDU: %s" % (ip))
    else:
        print ("login ok PDU: %s" % (ip))

    return agent


gSocket =  None
dest_ip = '127.0.0.1'

def initNetWork():
    hostname = socket.gethostname()  # 获取计算机名称
    dest_ip = socket.gethostbyname(hostname)  # 获取本机IP
    global gSocket
    gSocket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    if "192.168.1." in dest_ip:
        return True
    else:
        dest_ip = '127.0.0.1'
        # self.sendtoMainapp("Mac地址错误：" + mac, 0)

def sendtoMainApp(parameter, res , fn = 0):
    message = str(fn) + ";" + parameter + ";" + str(res)
    global gSocket
    gSocket.sendto(message.encode('utf-8-sig'), (dest_ip, 12306))




IpAddress = '192.168.1.162'
User = 'admin'
Password = 'Admin123'

def createAgent(ip, user, pw):
    return Agent("https", ip, user, pw, disable_certificate_verification=True)

def show_PDU_Info(agent):
    pdu = pdumodel.Pdu("/model/pdu/0", agent)
    metadata = pdu.getMetaData()
    print (metadata)
    ctrlBoardSerial = metadata.ctrlBoardSerial
    macAddress = metadata.macAddress
    hwRevision = metadata.hwRevision
    fwRevision = metadata.fwRevision
    print ('BOARD_SERIAL=%s'%ctrlBoardSerial)
    sendtoMainApp(ctrlBoardSerial , 1 , 1)

    print ('MAC=%s'%macAddress)
    sendtoMainApp(macAddress , 1 , 2)

    print ('HW=%s'%hwRevision)
    sendtoMainApp(hwRevision , 1 , 3)

    print ('FW=%s'%fwRevision)
    sendtoMainApp(fwRevision , 1 , 4)

def LCD_Button_Test(agent):
    '''
    LCD and Buttons test process is built in Firmware.
    You just need to start it. You need to follow the instruction in LCD to test it.
    When showing different colors, just press and release 'OK' button to confirm what color you see.
    '''
    suc = False
    retry = 5
    try:
        testdisplay = test.Display('/test/Display', agent)
        while retry > 0:
            if suc == True:
                break
            print('Enter LCD button test mode...')
            sendtoMainApp('第%d次进入LCD屏和按键测试模式，如果是颜色检测，请按OK按键'% (5 - retry + 1) , 1)
            testdisplay.enterTestMode()
            print('Please follow instruction on LCD panel to do test ...')
            sendtoMainApp('第%d次请按照步骤进行下面测试'% (5 - retry + 1), 1)
            time.sleep(6)
            while True:
                result =  testdisplay.getTestStatus()
                if result == test.Display.TestStatus.TEST_PASSED:
                    print('OK')
                    suc = True
                    sendtoMainApp('检测按键成功' , 1)
                    break
                elif result == test.Display.TestStatus.TEST_FAILED:
                    print('failed')
                    retry = retry - 1
                    sendtoMainApp('第%d次检测按键失败' %(5 - retry ), 0)
                    time.sleep(1)
                    break
                else:
                    time.sleep(1)
    except rpc.HttpException as e:
        print(str(e))
        sendtoMainApp(str(e), 0)
    return suc

def checkAndSend(text , ret):
    if(ret):
        sendtoMainApp(text+'成功', 1)
    else:
        sendtoMainApp(text+'失败', 0)

def USB_A_Test(agent):
    '''
    Use USB VID and PID to check how many USB device there.
    You can attach a USB stick into USB-A. Then check its PID and VID to verify USB-A port.
    '''
    cnt_devices = 2
    USB1_VID = 0x0424
    USB1_PID = 0x2513
    cnt_usb1 = 1
    USB2_VID = 0x1D6B
    USB2_PID = 0x0001
    cnt_usb2 = 1
    USB3_VID = 0x1D6B
    USB3_PID = 0x0002
    cnt_usb3 = 1

    suc = False
    try:
        testusb = usb.Usb("/test/Usb", agent)
        print('getting all usb descriptors ...')
        sendtoMainApp( '进入USB检测步骤 ...' , 1)
        usbdescs = testusb.getDevices()
        '''
        You can use this code to print out pid and vid of your test configuration.
        Then determine your test cases.
        ---------------------------
        print(len(usbdescs))
        for d in usbdescs:
            print '--------'
            print d.vendorId
            print d.productId
        '''
        #suc1 = check_device(usbdescs, USB1_VID, USB1_PID, cnt_usb1, 'checking #count of VID = 0x%04X PDI = 0x%04X'%(USB1_VID,USB1_PID))
        suc2 = check_device(usbdescs, USB2_VID, USB2_PID, cnt_usb2, 'checking count of VID = 0x%04X PDI = 0x%04X'%(USB2_VID,USB2_PID))
        text = '检测USB VID = 0x%04X PDI = 0x%04X'%(USB2_VID,USB2_PID)
        checkAndSend(text , suc2)

        suc3 = check_device(usbdescs, USB3_VID, USB3_PID, cnt_usb3, 'checking count of VID = 0x%04X PDI = 0x%04X'%(USB3_VID,USB3_PID))
        text = '检测USB VID = 0x%04X PDI = 0x%04X'%(USB3_VID,USB3_PID)
        checkAndSend(text , suc3)

        print('checking total device count ...')
        sendtoMainApp( '检查设备USB口的总数 ...' , 1)

        suc4 =check_count(cnt_devices, len(usbdescs))
        #suc = suc1 and suc2 and suc3 and suc4
        suc = suc2 and suc3 and suc4
        text = '检查设备USB口的总数'
        checkAndSend(text , suc)

    except rpc.HttpException as e:
        print (str(e))
        sendtoMainApp( str(e) , 0)
    return suc

def check_device(descs, vid, pid, expectcnt, text):
    print(text + " ...")
    sendtoMainApp(text + " ..." , 1)
    cnt = count_descs(descs, vid, pid)
    return check_count(expectcnt, cnt)

def check_count(expectcnt, cnt):
    if cnt == expectcnt:
        print('OK (%d)' % cnt)
        sendtoMainApp('检测USB口OK (%d)' % cnt , 1)
        suc = True
    else:
        print('failed (%d, expected %d)' % (cnt, expectcnt))
        sendtoMainApp('检测USB口失败的数量 (%d, 检测USB口的数量 %d)' % (cnt, expectcnt) , 1)
        suc = False
    return suc

def count_descs(descs, vid, pid):
    cnt = 0
    for d in descs:
        print('d.vendorId : %d ; d.productId ：%d' % ( d.vendorId , d.productId))
        if d.vendorId == vid and d.productId == pid:
            cnt += 1
    return cnt

def J1_Connection_Test(agent):
    '''
    Base board is a tool for J1 test.
    Read Base controller's serial number to determine the communication between Base and Node.
    3096445020 is just an example. You can find this serial number in browser (Maintenance -> Maintenance -> Controllers)
    '''
    suc = False
    try:
        pdu = pdumodel.Pdu("/model/pdu/0", agent)
        for controller in pdu.getControllers():
            metadata = controller.getMetaData()
            serialNumber = metadata.serial
            print("metadata.serial : %d " % (metadata.serial))
            if serialNumber == '3096445020':
                return True

    except rpc.HttpException as e:
        print(str(e))
    return suc

def discover():
    response = zeroconf.discover()
    ip = '192.168.1.100'
    for x in response:
        ip = x['ip']
        break
    return ip

if __name__=='__main__':
    try:
        IpAddress = discover()
        initNetWork()
        agent = createAgent(IpAddress, User, Password);
        show_PDU_Info(agent)
        sendtoMainApp('登录成功' , 1)
        ret = LCD_Button_Test(agent)
        print(ret)
        ret = USB_A_Test(agent)
        print(ret)
        #print(J1_Connection_Test(agent))
    except Exception as e:
        print (str(e))
        sendtoMainApp( str(e) , 0 )
        sendtoMainApp('登录失败' , 0 )
    sys.exit()







