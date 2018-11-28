# receive

import socket
import hashlib
import struct
import time
import pdb

BUFFER_SIZE = 1024
HEAD_STRUCT = '128sIq32s'
info_size = struct.calcsize(HEAD_STRUCT)

def cal_md5(file_path):
    with open(file_path, 'rb') as fr:
        md5 = hashlib.md5()
        md5.update(fr.read())
        md5 = md5.hexdigest()
        return md5

def unpack_file_info(file_info):
    file_name, file_name_len, file_size, md5 = struct.unpack(HEAD_STRUCT, file_info)
    file_name = file_name[:file_name_len]
    return file_name, file_size, md5

def recv_data(sock,size):
    data=''
#    pdb.set_trace()
    while True:
        try:
            c = sock.recv(1)
            if c == "":
                raise Exception()
            data += c
        except Exception as e:
            print e
            time.sleep(0.3)
            continue
        if size == len(data):
            return data
def recv_file(sock):
    try:
        file_info_package = recv_data(sock,info_size)
        file_name, file_size, md5_recv = unpack_file_info(file_info_package)

        recved_size = 0
        with open(file_name, 'wb') as fw:
            while recved_size < file_size:
                remained_size = file_size - recved_size
                recv_size = BUFFER_SIZE if remained_size > BUFFER_SIZE else remained_size
                recv_file = recv_data(sock,recv_size)
                recved_size += recv_size
                fw.write(recv_file.decode())
        md5 = cal_md5(file_name)
        if md5 != md5_recv:
            print ('MD5 compared fail!')
        else:
            print ('Received successfully')
    except Exception as e:
        print ("Socket error: %s" % str(e))

