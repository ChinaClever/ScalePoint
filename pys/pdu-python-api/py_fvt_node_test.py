#!/usr/bin/python3

import sys, time, csv, socket
from raritan import rpc
from raritan import zeroconf
#from raritan.rpc.pdumodel import *
from raritan.rpc import Agent, security , pdumodel, production , firmware, test, usb
from raritan.rpc import servermon, event, usermgmt, um, devsettings, pdumodel, cert, sensors

gSocket =  None
dest_ip = '127.0.0.1'

def initNetWork():
    global gSocket, dest_ip
    hostname = socket.gethostname()  # 获取计算机名称
    dest_ip = socket.gethostbyname(hostname)  # 获取本机IP
    gSocket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    if "192.168.1." in dest_ip:
        return True
    else:
        dest_ip = '127.0.0.1'

def sendtoMainApp(parameter, res = 1, fn = 0):
    global gSocket; #print(parameter)
    if(res):
        res = 1
    else:
        parameter = parameter + ' 异常'
    try:
        message = str(fn) + ";" + parameter + ";" + str(res)
        gSocket.sendto(message.encode('utf-8-sig'), (dest_ip, 12306))
    except Exception as e:
        print (e)


def discoverIpAddr():
    ip = '192.168.1.163'
    ips = zeroconf.discover()
    if(len(ips) > 0):
        for x in ips:
            ip = x['ip']
            break
        sendtoMainApp('自动发现IP地址：'+ip)
    else:
        sendtoMainApp('自动发现IP地址',0)
        print('use default IP: 192.168.1.163')
    return ip


User = 'admin'
Password = 'legrand'
IpAddress = '192.168.1.100'

def getOptArgvs():
    global User
    global Password
    global IpAddress
    num = len(sys.argv)
    if(num > 2):
        User = sys.argv[1]
        Password = sys.argv[2]
    if(num == 3) :
        IpAddress = discoverIpAddr()
    elif(num == 4) :
        IpAddress = sys.argv[3]

def createAgent():
    try:
        str = 'User=' + User + ' Password=' + Password + ' IpAddress=' + IpAddress
        agent = Agent("https", IpAddress, User, Password, disable_certificate_verification=True)
    except Exception as e:
        print (str(e))
        sendtoMainApp('登录失败：' + str , 0)
        
    return agent

def devLogin():
    initNetWork()
    getOptArgvs()
    return createAgent()

def show_PDU_Info(agent):
    pdu = pdumodel.Pdu("/model/pdu/0", agent)
    global User
    global Password
    global IpAddress
    str = 'User=' + User + ' Password=' + Password + ' IpAddress=' + IpAddress
    sendtoMainApp('登录成功：'+str)
    metadata = pdu.getMetaData(); print (metadata)
    nameplate = pdu.getNameplate();

    sendtoMainApp(metadata.ctrlBoardSerial , 1 , 1)
    sendtoMainApp(metadata.macAddress , 1 , 2)
    sendtoMainApp(metadata.hwRevision , 1 , 3)
    sendtoMainApp(metadata.fwRevision , 1 , 4)

    sendtoMainApp(nameplate.serialNumber , 1 , 5)
    sendtoMainApp(nameplate.manufacturer , 1 , 6)
    sendtoMainApp(nameplate.model , 1 , 7)

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
            time.sleep(0.6)
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
                    if(retry):
                        sendtoMainApp('第%d次检测按键失败' %(5 - retry ))
                    else:
                        sendtoMainApp('第%d次检测按键失败' %(5 - retry ), 0)
                    time.sleep(0.1)
                    break
                else:
                	time.sleep(0.1)
    except rpc.HttpException as e:
        print(str(e))
        sendtoMainApp(str(e), 0)
    return suc



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
        sendtoMainApp( '进入USB检测步骤 ...' )
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
        sendtoMainApp(text , suc2)

        suc3 = check_device(usbdescs, USB3_VID, USB3_PID, cnt_usb3, 'checking count of VID = 0x%04X PDI = 0x%04X'%(USB3_VID,USB3_PID))
        text = '检测USB VID = 0x%04X PDI = 0x%04X'%(USB3_VID,USB3_PID)
        sendtoMainApp(text , suc3)

        print('checking total device count ...')
        sendtoMainApp( '检查设备USB口的总数 ...')

        suc4 =check_count(cnt_devices, len(usbdescs))
        #suc = suc1 and suc2 and suc3 and suc4
        suc = suc2 and suc3 and suc4
        text = '检查设备USB口的总数'
        sendtoMainApp(text , suc)

    except rpc.HttpException as e:
        print (str(e))
        sendtoMainApp( str(e) , 0)
    return suc

def check_device(descs, vid, pid, expectcnt, text):
    print(text + " ...")
    sendtoMainApp(text + " ...")
    cnt = count_descs(descs, vid, pid)
    return check_count(expectcnt, cnt)

def check_count(expectcnt, cnt):
    if cnt == expectcnt:
        print('OK (%d)' % cnt)
        sendtoMainApp('检测USB口OK (%d)' % cnt)
        suc = True
    else:
        print('failed (%d, expected %d)' % (cnt, expectcnt))
        sendtoMainApp('检测USB口失败的数量 (%d, 检测USB口的数量 %d)' % (cnt, expectcnt))
        suc = False
    return suc

def count_descs(descs, vid, pid):
    cnt = 0
    for d in descs:
        if d.vendorId == vid and d.productId == pid:
            print('d.vendorId : %d ; d.productId : %d' % ( d.vendorId , d.productId))
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
            print("metadata.serial : %s " % (metadata.serial))
            #if serialNumber == '3096445020':
            if serialNumber == '180149487':
                sendtoMainApp('检测J1口OK (%s)' % serialNumber)
                return True
            else:
                sendtoMainApp('检测J1口Failed' , 0 )

    except rpc.HttpException as e:
        print(str(e))
    return suc

def changeDefaultPassword(agent):
    '''
    After calling factory hard reset, password becomes default 'legrand'.
    PDU firmware fources user to change passowrd to enable the other JSON-RPC interfaces.
    The method is to change password to 'Raritan1', disable strong password request and change password back to default 'legrand'
    '''
    try:
        # change password to Raritan1
        user = usermgmt.User("/auth/user/admin", agent)
        idlRet = user.setAccountPassword('Raritan1')
        time.sleep(1)
        # create new agent using new password Raritan1
        global Password
        Password = 'Raritan1'
        agent = createAgent()
        # disable strong passowrd request
        sec = security.Security("/security", agent)
        pwset = sec.getPwSettings()
        pwset.enableStrongReq = False
        sec.setPwSettings(pwset)
        time.sleep(1)
        # change password to back to original one
        user = usermgmt.User("/auth/user/admin", agent)
        Password = 'legrand'
        idlRet = user.setAccountPassword(Password)
        # return new agent with original password
        return createAgent()
    except rpc.HttpException as e:
        global User
        global IpAddress
        str = 'User=' + User + ' Password=' + Password + ' IpAddress=' + IpAddress
        sendtoMainApp('登录失败：' + str , 0)
        print(e)
    return None
    
def performFactoryHardReset(agent):
    suc = False
    try:
        fw = firmware.Firmware("/firmware", agent)
        print('Enter production mode')
        sendtoMainApp('进入恢复出厂设置模式...')
        prod = production.Production('/production', agent)
        prod.enterFactoryConfigMode('Cr4sh&8urn')
        print('sending factory defaults command ...')
        sendtoMainApp('发送恢复出厂设置命令...')
        if (fw.hardFactoryReset() == 0):
            time.sleep(2)
            print('OK')
            suc = True
            sendtoMainApp('恢复出厂设置成功')
        else:
            print('Factory hard reset failed')
            sendtoMainApp('恢复出厂设置', 0)
    except rpc.HttpException as e:
        print(str(e))
    return suc

def close():
    try:
        time.sleep(0.1)
        gSocket.close()
    except:
        sendtoMainApp('测试脚本执行完成')
    finally:
        time.sleep(0.3)


if __name__=='__main__':
    try:
        agent = devLogin()
        agent = changeDefaultPassword(agent)
        if(None != agent):
            show_PDU_Info(agent)
            ret = LCD_Button_Test(agent)
            ret = USB_A_Test(agent)
            ret = J1_Connection_Test(agent)
            ret = performFactoryHardReset(agent)
    except Exception as e:
        str = 'User=' + User + ' Password=' + Password + ' IpAddress=' + IpAddress
        sendtoMainApp('登录失败：' + str , 0)
        print (e)
    finally:
        close()
    sys.exit()







