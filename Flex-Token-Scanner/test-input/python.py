
#python keywords:
#and,
#as,
#assert,
#break,
#class,
#continue,
#def,
#del,
#elif,
#else,
#except,
#exec,
#finally,
#for,
#from,
#global,
#if,
#import,
#in,
#is,
#lambda,
#not,
#or,
#pass,
#print,
#raise,
#return,
#try,
#while,
#with,
#yield,



    #coding=utf-8  
    import urllib.request  
    import re  
    import os  
      
    '''
    Urllib 模块提供了读取web页面数据的接口，我们可以像读取本地文件一样读取www和ftp上的数据 
    urlopen 方法用来打开一个url 
    read方法 用于读取Url上的数据 
    '''  
      
    def getHtml(url):  
        page = urllib.request.urlopen(url);  
        html = page.read();  
        return html;  
      
    def getImg(html):  
        imglist = re.findall('img src="(http.*?)"',html)  
        return imglist  
      
    html = getHtml("https://www.zhihu.com/question/34378366").decode("utf-8");  
    imagesUrl = getImg(html);  
      
    if os.path.exists("D:/imags") == False:  
        os.mkdir("D:/imags");  
          
    count = 0;  
    for url in imagesUrl:  
        print(url)  
        if(url.find('.') != -1):  
            name = url[url.find('.',len(url) - 5):];  
            bytes = urllib.request.urlopen(url);  
            f = open("D:/imags/"+str(count)+name, 'wb');  
            f.write(bytes.read());  
            f.flush();  
            f.close();  
            count+=1;  


def sqrt_binary(num):  
    x=sqrt(num)  
    y=num/2.0  
    low=0.0  
    up=num*1.0  
    count=1  
    while abs(y-x)>0.00000001:  
        print count,y  
        count+=1          
        if (y*y>num):  
            up=y  
            y=low+(y-low)/2  
        else:  
            low=y  
            y=up-(up-y)/2  
    return y 
