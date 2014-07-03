#! /usr/bin/env python
# -*- coding: utf-8 -*-

'''Import Tile Data From MOngodb To Qiniu Storage'''
__author__ = 'anzhj'

import sys
import cStringIO
import pymongo
import base64
from qiniu import rs
from qiniu import conf
from qiniu import io


# config
conf.ACCESS_KEY = os.getenv("QINIU_ACCESS_KEY")
conf.SECRET_KEY = os.getenv("QINIU_SECRET_KEY")
bucket_name = os.getenv("QINIU_BUCKET")

restservicename = "castle"
mkeyprifix = "rest/services/3DTileService/datasources"
tkeyprifix = mkeyprifix + "/" + restservicename + "/tiles/"
mongohost = "127.0.0.1"
mongoport = 27017
mongodb = "castle0701JPG"

policy = rs.PutPolicy(bucket_name)
uptoken = policy.token()
extra = io.PutExtra()
extra.mime_type = "application/octet-stream"

class Up2qn:
	

	def __init__(self):
		'''Initializes the upload tools.'''
		print 'Initializing up2qn.\n'
		pass

	def __del__(self):
		'''Unitializes upload tools'''
		print 'Unitializes.\n'

	def writeQnTile(self, key, mf):
		ret, err = io.put(uptoken, key, mf, extra)
		if err is not None:
			sys.stderr.write('error: %s ' % err)
			print err
		print ret

	def writeQnM(self, strdata):
		tmp = base64.encodestring(strdata)
		mf = cStringIO.StringIO(tmp)
		sKey = '%s%s' % (mkeyprifix, restservicename)
		self.writeQnTile(sKey, mf)

	def writeQnN(self, key, strdata):
		mf = cStringIO.StringIO(strdata)
		sKey = '%s%s' % (tkeyprifix, key)
		self.writeQnTile(sKey, mf)
	
	def run(self):
		client = pymongo.MongoClient(mongohost, mongoport)
		db = client[mongodb]
		col = db.fde_gmap
		cursor = col.find({},{"_id":0, "id":1, "mp":1})
		for doc in cursor:
			key = doc["id"]
			data = doc["mp"]

			if key != "m":
				self.writeQnN(key, data)
			else:
				self.writeQnM(data)


runner = Up2qn()
runner.run()

