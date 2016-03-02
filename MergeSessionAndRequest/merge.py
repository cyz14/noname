# -*- coding: utf-8 -*-

# 合并思路是先分别对 requests 和 sessions 按照 SessionId 进行升序排序，
# 再分别扫描合并相同 SessionId 的 Session 和 Requests，同时忽略了
# 只有 request 或 只有 session 的情况.

import os, json

def fileLineNumber_len(file_name):
    # can be very slow and memory costing
    file = open(file_name, 'r')
    return len(file.readlines())

def sign(number):
    ''' return the sign of number '''
    if number > 0:
        return 1
    elif number < 0:
        return -1
    return 0


def sessionCmp(SesA, SesB):
    ''' Compare function for sessions '''
    SesA = json.loads(SesA)
    SesB = json.loads(SesB)
    tmp = int(SesA['SessionId']) - int(SesB['SessionId'])
    return sign(tmp)


def sort_sessions(session_file_name):
    ''' Sort the sessions file '''
    sessions = open(session_file_name, 'r+').readlines()
    sessions.sort(sessionCmp)
    
    sorted_sessions = open('sorted_' + session_file_name, 'w')
    sorted_sessions.writelines(sessions)
    sorted_sessions.close()


def requestCmp(ReqA, ReqB):
    ''' Compare function for requests '''
    reqa = json.loads(ReqA)
    reqb = json.loads(ReqB)
    tmp  = int(reqa['SessionId']) - int(reqb['SessionId'])
    return sign(tmp)


def sort_requests(request_file_name):
    ''' Sort the requests file '''
    freq = open(request_file_name, 'r+')
    requests = freq.readlines()
    requests.sort(requestCmp)
    sorted_requests = open('sorted_' + request_file_name, 'w')
    sorted_requests.writelines(requests)
    sorted_requests.close()

    

def merge(ses_file, req_file):
    fses = open('sorted_'+ses_file, 'r')
    freq = open('sorted_'+req_file, 'r')
    fmerged = open('merged_' + ses_file[:-5] + '_' + req_file[:-5] + '.json', 'w')
    f_extra_req = open('extra_req.json', 'w')
    
    done = False
    req_line = ''
    req = json.loads('{}')
    
    for line in fses:
        try:
            newlog = json.loads("{}")
            ses = json.loads(line)
            newlog['SesLog'] = ses
        except Exception, e:
            print "Session file format error."
            print "Exception", e
            continue
        # print newlog['SesLog']
        newlog['ReqLog'] = []
        
        # print ses
        sesid = int(ses['SessionId'])
        keep_alive_num = int(ses['KeepAliveNum'])
        # print '%s: %d, %s: %d' % ('SessionId', sesid, ' KeepAliveNum', keep_alive_num)
        
        merged = False
        while keep_alive_num:
            if not req_line:
                req_line = freq.readline()
                if not req_line:
                    done = True
                    break
            
            req = json.loads(req_line)
            req_ses_id = int(req['SessionId'])
            if req_ses_id == sesid:
                merged = True
                newlog['ReqLog'].append(json.loads(req_line))
                keep_alive_num -= 1
                req_line = ''
            elif req_ses_id < sesid:
                f_extra_req.write(req_line)
                req_line = ''
                continue
            else:
                break        
        
        if len(newlog['ReqLog']) == int(newlog['SesLog']['KeepAliveNum']):
            fmerged.write(json.dumps(newlog, separators=(',', ':'))+'\n')
        # 测试时输出
        # print json.dumps(newlog, indent = 4)
        if done:
            break
        
    fmerged.close()
        

def getFileNamesFromDir(dir):
    id = 0
    file_list = []
    for file_name in os.listdir(dir):
        if os.path.isfile(os.path.join(dir, file_name)):
            print "%3d: %s" % (id, file_name)
            id += 1
            file_list.append(file_name)

    ses_id = int(raw_input('Please input session file number: '))
    print ses_id, type(ses_id)
    req_id = int(raw_input('Please input request file number: '))
    return file_list[ses_id], file_list[req_id]

def main():
    ses_file, req_file = getFileNamesFromDir(os.getcwd())
    print ses_file, req_file
    # python的排序耗费了不少时间
    sort_sessions(ses_file)
    sort_requests(req_file)
    merge(ses_file, req_file)


if __name__ == '__main__':
    main()
