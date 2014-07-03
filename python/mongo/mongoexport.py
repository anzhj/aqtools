#! /usr/bin/env python
import pymongo
client = pymongo.MongoClient("10.144.166.152", 40001)
db = client.singapore0625JPG
print (db.collection_names())
col = db.fde_gmap
cursor = col.find_one({"id":"0000000504000000000000000F00000002"},{"_id":0,"mp":1})
fw=open('/root/test.dat', 'wb')
fw.write(cursor["mp"])
fw.close
