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
from qiniu import rsf


# config
conf.ACCESS_KEY = os.getenv("QINIU_ACCESS_KEY")
conf.SECRET_KEY = os.getenv("QINIU_SECRET_KEY")
bucket_name = os.getenv("QINIU_BUCKET")
restservicename = "castle"
mkeyprifix = "rest/services/3DTileService/datasources"
tkeyprifix = mkeyprifix + "/" + restservicename + "/tiles/"

policy = rs.PutPolicy(bucket_name)
uptoken = policy.token()
extra = io.PutExtra()
extra.mime_type = "application/octet-stream"

class RemoveQn:
	def __init__(self):
		'''Initializes the upload tools.'''
		print 'Initializing up2qn.\n'
		pass

	def __del__(self):
		'''Unitializes upload tools'''
		print 'Unitializes.\n'

	def deleteTile(self, key):
		rsc = rs.Client()
		ret, err = rsc.delete(bucket_name, key)
		if err is not None:
			sys.stderr.write('error: %s ' % err)
			print err
		print ret

	def run(self):
		marker = None
		err = None
		rs = rsf.Client()
		while err is None:
			ret, err = rs.list_prefix(bucket_name, prefix="rest", limit=10, marker=marker)
			marker = ret.get('marker', None)
			for item in ret['items']:
				print item
				self.deleteTile(item["key"])

		if err is not rsf.EOF:
			print err


runner = RemoveQn()
runner.run()

