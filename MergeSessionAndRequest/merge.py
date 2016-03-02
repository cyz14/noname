# -*- coding: utf-8 -*-

import json, time, os, sys

EMPTYJSON = '{}'
d_log = {}	# dict to save new logs

def getFileNamesFromDir(dir):
	 # get session file name and request file name from current work directory 
    id = 0
    file_list = []
    for file_name in os.listdir(dir):
        if os.path.isfile(os.path.join(dir, file_name)):
            print "%3d: %s" % (id, file_name)
            id += 1
            file_list.append(file_name)

    ses_id = int(raw_input('Please input session file number: '))
    print ses_id, file_list[ses_id]
    req_id = int(raw_input('Please input request file number: '))
    print req_id, file_list[req_id]

    return file_list[ses_id], file_list[req_id]


def main():
	ses_file, req_file = getFileNamesFromDir(os.getcwd())
	merge(ses_file, req_file)


def fileLineNumber(file_name):
	# Return the number of lines of file, by enumerate over file's lines.
    count = -1
    for count, line in enumerate(open(file_name, 'r')):
        pass
    return count + 1


def merge(f_ses_name, f_req_name):
	print time.clock(), 'Program starts.'
	# ses_line_num = fileLineNumber(f_ses_name)
	# req_line_num = fileLineNumber(f_req_name)
	# print time.clock()
	# print 'Session file number:', ses_line_num
	# print 'Request file number:', req_line_num

	f_ses = open(f_ses_name, 'r')
	f_req = open(f_req_name, 'r+')

	# result file
	f_ans = open(f_ses_name[:-4]+f_req_name[:-4]+'json', 'w')
	
	print 'Reading sessions file...'
	for cnt_ses, line in enumerate(f_ses):
		session = json.loads(line)
		session_id = int(session['SessionId'])
		d_log[session_id] = json.loads(EMPTYJSON)
		d_log[session_id]['SesLog'] = session
		d_log[session_id]['ReqLog'] = []
		# update progress can be very time consuming because of IO in loop
		# if cnt_ses % 100 == 0:
			# sys.stdout.write('\rProgress: %.1f %% ' % (1.0 * cnt_ses / ses_line_num * 100))

	# sys.stdout.write('\rProgress: %.1f %% ' % 100.0)
	f_ses.close()
	print '\nUsed %.6fs to read sessions file.' % time.clock()

	print 'Reading requests file...'
	for cnt_req, line in enumerate(f_req):
		request = json.loads(line)
		request_id = int(request['SessionId'])
		req_time = int(request['Timestamp']) * 1000
		if request_id in d_log:
			# when SessionId has collisions, we should check time range
			# if req_time <= int(d_log[request_id]['SesLog']['SesStartTime']) + int(d_log[request_id]['SesLog']['SesAllTime']):
			d_log[request_id]['ReqLog'].append(request)
			if len(d_log[request_id]['ReqLog']) == int(d_log[request_id]['SesLog']['KeepAliveNum']):
				f_ans.write(json.dumps( d_log.pop(request_id), separators=(',', ':') ) + '\n')

		# update progress can be very time consuming because of IO in loop
		# if cnt_req % 100 == 0:
			# sys.stdout.write('\rProgress: %.1f %% ' % (1.0 * cnt_req / req_line_num * 100))
	
	f_req.close()
	print '\nUsed %.6fs to read requests file.\n' % time.clock()

	d_log.clear()

if __name__ == '__main__':
	main()
