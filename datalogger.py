import asyncio
from bleak import BleakClient
import numpy as np
import sys, os, time

## for XIAO nRF52840 Sense
## A:    C1:73:C6:56:D9:C6, name: RatTrackA, uuid: ['845c8a00-0000-4fcb-a990-64f508f3781e']
## B:    D4:17:D7:95:52:ED, name: RatTrackB, uuid: ['845c8a01-0000-4fcb-a990-64f508f3781e']
## C:    C7:C5:1F:11:2C:D6, name: RatTrackC, uuid: ['845c8a02-0000-4fcb-a990-64f508f3781e']
## D:    DE:43:87:EF:AF:B4, name: RatTrackD, uuid: ['845c8a03-0000-4fcb-a990-64f508f3781e']
## E:    EE:A8:01:48:78:B1, name: RatTrackE, uuid: ['845c8a04-0000-4fcb-a990-64f508f3781e']
## F:    D8:36:A1:8C:37:92, name: RatTrackF, uuid: ['845c8a05-0000-4fcb-a990-64f508f3781e']
## G:    D5:29:E7:9B:56:FA, name: RatTrackF, uuid: ['845c8a06-0000-4fcb-a990-64f508f3781e']
## H:    DD:B4:45:64:C6:26, name: RatTrackF, uuid: ['845c8a07-0000-4fcb-a990-64f508f3781e']
## Test: FD:4B:26:0E:30:0D, name: Test, uuid: ['845c8a88-0000-4fcb-a990-64f508f3781e']
mac_address = "FD:4B:26:0E:30:0D"
Name="Test"
UUIDServ='845c8a88-0000-4fcb-a990-64f508f3781e'
UUIDChar='845c8a88-0010-4fcb-a990-64f508f3781e'


sAddress=["C1:73:C6:56:D9:C6", "D4:17:D7:95:52:ED", "C7:C5:1F:11:2C:D6", "DE:43:87:EF:AF:B4", "EE:A8:01:48:78:B1", "D8:36:A1:8C:37:92", "D5:29:E7:9B:56:FA", "DD:B4:45:64:C6:26"];
sName   =["RatTrackA",         "RatTrackB",         "RatTrackC",         "RatTrackD",         "RatTrackE",         "RatTrackF",         "RatTrackG",         "RatTrackH"];
sUuidS  =['845c8a00-0000-4fcb-a990-64f508f3781e', '845c8a01-0000-4fcb-a990-64f508f3781e', '845c8a02-0000-4fcb-a990-64f508f3781e', '845c8a03-0000-4fcb-a990-64f508f3781e', '845c8a04-0000-4fcb-a990-64f508f3781e', '845c8a05-0000-4fcb-a990-64f508f3781e', '845c8a06-0000-4fcb-a990-64f508f3781e', '845c8a07-0000-4fcb-a990-64f508f3781e']
sUuidC  =['845c8a00-0010-4fcb-a990-64f508f3781e', '845c8a01-0010-4fcb-a990-64f508f3781e', '845c8a02-0010-4fcb-a990-64f508f3781e', '845c8a03-0010-4fcb-a990-64f508f3781e', '845c8a04-0010-4fcb-a990-64f508f3781e', '845c8a05-0010-4fcb-a990-64f508f3781e', '845c8a06-0010-4fcb-a990-64f508f3781e', '845c8a07-0010-4fcb-a990-64f508f3781e']
##NUM = 2
##mac_address=sAddress[NUM]
##Name=sName[NUM]
##UUIDServ=sUuidS[NUM]
##UUIDChar=sUuidC[NUM]
if len(sys.argv) == 2:
  NUM = int(sys.argv[1])
  mac_address=sAddress[NUM]
  Name=sName[NUM]
  UUIDServ=sUuidS[NUM]
  UUIDChar=sUuidC[NUM]

def notification_handler(sender, data: bytearray):
    float_array = np.frombuffer(data, dtype=np.float32)
    Text=str(float_array)+'\n';
    with open("datalog_"+Name+".txt",'a') as file1:
        file1.write(Text)

async def run(address, loop):
    async with BleakClient(address, timeout=20.0, loop=loop) as client:
        x = client.is_connected
        with open("syslog_"+Name+".txt",'a') as file2:
          file2.write("Client "+str(x)+" is connected")
        await client.start_notify(UUIDChar, notification_handler)

        while True:
            await asyncio.sleep(1)

  
loop = asyncio.new_event_loop() 
loop.run_until_complete(run(mac_address, loop))
