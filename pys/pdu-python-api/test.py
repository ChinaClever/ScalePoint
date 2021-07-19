#!/usr/bin/python3

import sys, time, csv
from raritan import rpc
from raritan import zeroconf
from raritan.rpc.pdumodel import *
from raritan.rpc import Agent, pdumodel, firmware
from raritan.rpc import servermon, event, usermgmt, um, devsettings, pdumodel, cert, sensors

def devIpAddr():
    ips = zeroconf.discover();
    if(len(ips) > 0): return ips[0]
    else: print('use default IP: 192.168.1.100')
    return '192.168.1.100'

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










print("AAAAA")

agent = login()
pdu = pdumodel.Pdu("/model/pdu/0", agent)
metadata = pdu.getMetaData()
print(metadata)







