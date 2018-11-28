#!/usr/bin/env python
#_*_coding:utf-8_*_
#centos install pandas
#yum -y install python-pandas
#
import pandas as pd
import sys
import os
import matplotlib.pyplot as plt
import pdb

TIMEFORMAT2 = '%Y-%m-%d %H:%M:%S'
pd.set_option('display.width', 120)  #设置显示列宽
pd.set_option('display.max_rows',None) #示全部数据
data_path = "."
original_file = ""
node_data_file = ""
data_path = ""
colorlist = [
'#D2691E',
'#6495ED',
'#DC143C',
'#00008B',
'#008B8B',
'#B8860B',
'#A9A9A9',
'#006400',
'#BDB76B',
'#8B008B',
'#556B2F',
'#9932CC',
'#8B0000',
'#E9967A',
'#8FBC8F',
'#483D8B',
'#2F4F4F',
'#00CED1',
'#9400D3',
'#00BFFF',
'#696969',
'#1E90FF',
'#228B22',
'#DCDCDC',
'#FFD700',
'#DAA520',
'#808080',
'#008000',
'#ADFF2F',
'#FF69B4',
'#CD5C5C',
'#4B0082',
'#F0E68C',
'#E6E6FA',
'#7CFC00',
'#FFFACD',
'#ADD8E6',
'#F08080',
'#FAFAD2',
'#90EE90',
'#D3D3D3',
'#FFB6C1',
'#FFA07A',
'#20B2AA',
'#87CEFA',
'#778899',
'#B0C4DE',
'#00FF00',
'#32CD32',
'#FAF0E6',
]
# origin 与 node-data 数据之间进行比较
def origin_output_statistics(original_file, node_data_file):
    df_node = pd.read_csv(node_data_file, delimiter="|", names=['id',"node", "power", "qps", "time"])
    df_origin = pd.read_csv(original_file, delimiter="|", names=['original_power', "power", "time"])
    
    #df_node["datetime"] = pd.to_datetime(df_node["time"], format=TIMEFORMAT2)
    
    df_sum=df_node.groupby("id").sum()
    #print df_sum['power']
    #print df_origin['power']
    #print df_sum['power'].sum()
    #df_out = df_sum['power'].iloc[1:].as_matrix() - df_origin['power'].iloc[:-1].as_matrix()
    #df_out = df_sum['power'].reset_index() - df_origin['power'].reset_index()
    #df_origin.loc[len(df_origin)] = 0

    #for i in range(len(df_origin) - 2,0,-1):
    #    df_origin.loc[i+1] = df_origin.loc[i]
    #df_origin.loc[0] = None
    
    a= (df_origin.reset_index()['power'] - df_sum.reset_index()['power'])
    b= 1.0 * a / df_origin['power'].sum()
    print "----------------"*3
    print "(set_value - output_value) / set_value.sum()"
    temp = pd.concat([df_origin.reset_index()['power'], df_sum.reset_index()['power'], a, b],axis=1)
    #temp = pd.concat([df_origin.reset_index()['power'], df_sum.reset_index()['power'], a, b],axis=1)
    temp.columns = ['set_value','output_value','diff',"ratio"]
    temp.loc['sum'] = temp.apply(lambda x: x.sum())
    print temp
    print "----------------"*3

#node-data 数据批次比较
def bat_statistics(path):
    dfs= []
    namelist = []
    index = 0
    for  root,dir,name in os.walk(path):
        for f in name:
            if f.find("node") <> -1:
                print "["+str(index)+"]", f
                index += 1
                df=pd.read_csv(os.path.join(path,f), delimiter="|", names=['id',"host", "power", "qps", "time"])
                df['QPS']=df['qps'].map(lambda x : float(x.replace("M",'')))            
                dfs.append(df.groupby("id").sum())
                namelist.append(f)

    x = raw_input("Please enter the baseline file index:")
    if int(x) < 0 or int(x) >= len(dfs):
        print "input error!\n"
        quit(1)
    if int(x) > 0:
        baseline = dfs[int(x)]
        tmp0 = dfs[0]
        dfs[0],dfs[int(x)] = baseline,tmp0 
        baseline = namelist[int(x)]
        tmp0 = namelist[0]
        namelist[0],namelist[int(x)] = baseline,tmp0 
    print "--------------------"*len(dfs)
    for i in range(0,len(dfs)):
        dfs[i].columns = ["power","-Task"+str(i+1)+"- QPS"]
        dfs[i].loc['sum'] = dfs[i].apply(lambda x: x.sum())
    print pd.concat(dfs,axis=1)
    print "--------------------"*len(dfs)
    dfs[0].columns = ["power","QPS"]
    dfs[0].drop('sum',axis=0,inplace=True)
    print namelist
    for i in range(1,len(dfs)):
        dfs[i].columns = ["power","QPS"]
        dfs[i].drop('sum',axis=0,inplace=True)
        print "[(Task"+str(i+1)+" - Task1)].sum() / Task1.sum():"
        print (1.0*(dfs[i]-dfs[0]).abs().sum()/dfs[0].sum())
        print "="*40

def bat_plot(path):
    dfs= []
    namelist = []
    for  root,dir,name in os.walk(path):
        for f in name:
            if f.find("node") <> -1:
                df=pd.read_csv(os.path.join(path,f), delimiter="|", names=['id',"host", "power", "qps", "time"])
                df['QPS']=df['qps'].map(lambda x : float(x.replace("M",'')))            
                dfs.append(df.groupby("id").sum())
                namelist.append(f.replace(".txt","").replace("node-data-","").replace("-",""))

    fig = plt.figure()
    ax1 = fig.add_subplot(211)
    ax2 = fig.add_subplot(212)
    ax1.grid(True)
    ax2.grid(True)
    '''
    for i in range(0,len(dfs)):
        dfs[i].columns = ["["+str(i+1)+"] power","["+str(i+1)+"] QPS"]
    groupdf = pd.concat(dfs,axis=1)

    groupdf.plot(linewidth='3',label="power",marker="o",markersize=10)
    '''

    for index in range(0,len(namelist)):
        ax1.plot(dfs[index]['power'],linewidth='1',label="power-" + namelist[index] ,marker="o",markersize=3)
        ax2.plot(dfs[index]['QPS'],linewidth='1',label="QPS-" + namelist[index] ,marker="v",markersize=3)
    
    ax1.set_ylabel("Power")
    ax2.set_ylabel("QPS")
    ax1.legend(loc='upper center', bbox_to_anchor=(0.5,1.1),ncol=4,fancybox=True,shadow=True)
    ax2.legend(loc='upper center', bbox_to_anchor=(0.5,1.1),ncol=4,fancybox=True,shadow=True)
    plt.show()


def single_plot(path):
    fn = []
    dfs = []
    for  root,dir,name in os.walk(path):
        for f in name:
            if f.find("node") <> -1:
                print f
                fn.append(f.replace("node-data-","").replace(".txt",""))
                df=pd.read_csv(os.path.join(path,f), delimiter="|", names=['id',"host", "power", "qps", "time"])
                df['QPS']=df['qps'].map(lambda x : float(x.replace("M",'')))            
                #df['power']=df['power'].map(lambda x : float(x*50))            
		dfs.append(df)
    dfg = dfs[0].groupby("host")
    fig = plt.figure()
    ax1 = fig.add_subplot(111)
    ci = 1
    for name, group in dfg:
        for j in range(0,len(dfs)):
            dg = dfs[j][dfs[j]['host']==name]
            ax1.plot(dg['power'],color=colorlist[ci],linewidth='2',label="power-" + name + "-" + fn[j],marker="o",markersize=3)
            ax2 = ax1.twinx()
            ax2.plot(dg['QPS'],color=colorlist[ci+1],linewidth='2',label="QPS-" + name + "-" + fn[j],marker="v",markersize=3)
            ax1.set_ylabel("Power")
            ax2.set_ylabel("QPS")
            legend1 = ax1.legend(loc='upper center', bbox_to_anchor=(0.5,1.05),ncol=4,fancybox=True,shadow=True)
            legend2 = ax2.legend(loc='upper center', bbox_to_anchor=(0.5,1.1),ncol=4,fancybox=True,shadow=True)
            ci += 2

        plt.grid(True)
        plt.show()
    #plt.grid(True)
    #plt.legend(loc='best')
    #plt.show()


try:
    print sys.argv
    if len(sys.argv) == 1:
        bat_statistics(".")
    elif len(sys.argv) == 2:
        data_path = sys.argv[1]
        #single_plot(data_path)
        bat_plot(data_path)
        bat_statistics(data_path)
    elif len(sys.argv) == 3:
        origin_output_statistics(sys.argv[1],sys.argv[2])
    else:
        print "%s [path | origin_file node_data_file]" % sys.argv[0]
        quit()
except Exception as e:
    print e
