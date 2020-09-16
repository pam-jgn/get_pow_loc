import paramiko
import time

tablename_tag1 = "tag1_result_location"
tablename_tag2 = "tag2_result_location"
tablename_tag3 = "tag3_result_location"
tablename_tag4 = "tag4_result_location"
tablename_tag5 = "tag4_result_location"
tablename_pow = "powerconsumption_table"

tag1_ip = '192.168.100.128'
tag2_ip = '192.168.100.126'
tag3_ip = '192.168.100.127'
tag4_ip = '192.168.100.129'
tag5_ip = '192.168.100.122'

def start_Tag(ip, tablename):
     tag_ip = '%s' % (ip)
     tag_user = 'pi'
     tag_passwd = 'pi'
     tag_resetcommand = 'ps aux | grep ./tag_cfg | grep -v grep | awk \'{ print \"kill -9\", $2 }\' | sh'
     tag_command1 = 'cd -P /home/pi/Desktop/DWM1001_DWM1001-DEV_MDEK1001_Sources_and_Docs_v8/DWM1001/Source_Code/DWM1001_host_api/dwm1001_host_api/examples/ex1_TWR_2Hosts/tag; ./tag_cfg'
     tag_client = paramiko.SSHClient()
     tag_client.load_system_host_keys()
     tag_client.set_missing_host_key_policy(paramiko.AutoAddPolicy())
     tag_conn = tag_client.connect(tag_ip, 22, tag_user, tag_passwd)
     tag_stdin, tag_stdout, tag_stderr = tag_client.exec_command(tag_resetcommand) 
     time.sleep(0.3)
     tag_stdin, tag_stdout, tag_stderr = tag_client.exec_command(tag_command1) 
     ##テーブル名指定
     tag_stdin.write('%s\n' % (tablename))

def start_Pow():
    pow_ip = '192.168.100.101'
    pow_user = 'pi'
    pow_passwd = 'raspberry'
    pow_resetcommand = 'ps aux | grep collecting_pow.py | grep -v grep | awk \'{ print \"kill -9\", $2 }\' | sh'
    pow_command = 'python collecting_pow.py'
    pow_client = paramiko.SSHClient()
    pow_client.load_system_host_keys()
    pow_client.set_missing_host_key_policy(paramiko.AutoAddPolicy())
    pow_conn = pow_client.connect(pow_ip,22,pow_user ,pow_passwd)
    pow_stdin, pow_stdout, pow_stderr = pow_client.exec_command(pow_resetcommand)
    time.sleep(0.3)
    pow_stdin, pow_stdout, pow_stderr = pow_client.exec_command(pow_command)


# ---  main --# 
if __name__ == "__main__": 
    start_Pow()
    time.sleep(10)
  #  start_Tag(tag1_ip, tablename_tag1)
  #  start_Tag(tag2_ip, tablename_tag2)
  #  start_Tag(tag3_ip, tablename_tag3)
    start_Tag(tag4_ip, tablename_tag4)
  #  start_Tag(tag5_ip, tablename_tag5)
    time.sleep(6)
  