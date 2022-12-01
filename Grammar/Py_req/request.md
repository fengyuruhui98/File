# Python request 库


- [Python request 库](#python-request-库)
  - [1.什么是Requests](#1什么是requests)
  - [2.安装Requests库](#2安装requests库)
  - [3.各种请求方式](#3各种请求方式)
    - [3.1 基本的GET请求](#31-基本的get请求)
    - [3.2带参数的GET请求](#32带参数的get请求)
    - [3.3解析json](#33解析json)
    - [3.4获取二进制数据](#34获取二进制数据)
    - [3.5添加headers](#35添加headers)
    - [3.6基本POST请求](#36基本post请求)
  - [4.响应](#4响应)
    - [4.1response属性](#41response属性)
  - [5.高级操作](#5高级操作)
    - [5.1文件上传](#51文件上传)
    - [5.2获取cookie](#52获取cookie)
    - [5.3会话维持、模拟登陆](#53会话维持模拟登陆)
    - [5.4证书验证](#54证书验证)
    - [5.5代理设置](#55代理设置)
    - [5.6超时设置](#56超时设置)
    - [5.7认证设置](#57认证设置)
    - [5.8异常处理](#58异常处理)

## 1.什么是Requests

Requests 是⽤Python语⾔编写，基于urllib，采⽤Apache2 Licensed开源协议的 HTTP 库。它⽐ urllib 更加⽅便，可以节约我们⼤量的⼯作，完全满⾜HTTP测试需求。

⼀句话——Python实现的简单易⽤的HTTP库

## 2.安装Requests库
进入命令行win+R执行

命令：pip install requests

项目导入：import requests

## 3.各种请求方式

直接上代码，不明白可以查看我的urllib的基本使用方法
```py
import requests
requests.post('http://httpbin.org/post')
requests.put('http://httpbin.org/put')
requests.delete('http://httpbin.org/delete')
requests.head('http://httpbin.org/get')
requests.options('http://httpbin.org/get')
```
这么多请求方式，都有什么含义，所以问下度娘：

- GET： 请求指定的页面信息，并返回实体主体。

- HEAD： 只请求页面的首部。

- POST： 请求服务器接受所指定的文档作为对所标识的URI的新的从属实体。

- PUT： 从客户端向服务器传送的数据取代指定的文档的内容。

- DELETE： 请求服务器删除指定的页面。

get 和 post比较常见 GET请求将提交的数据放置在HTTP请求协议头中；POST提交的数据则放在实体数据中

### 3.1 基本的GET请求
```py
import requests
 
response = requests.get('http://httpbin.org/get')
print(response.text)
```
返回值：
```py
{
  "args": {}, 
  "headers": {
    "Accept": "*/*", 
    "Accept-Encoding": "gzip, deflate", 
    "Connection": "close", 
    "Host": "httpbin.org", 
    "User-Agent": "python-requests/2.18.4"
  }, 
  "origin": "183.64.61.29", 
  "url": "http://httpbin.org/get"
}
```
### 3.2带参数的GET请求

将name和age传进去
```py
import requests
response = requests.get("http://httpbin.org/get?name=germey&age=22")
print(response.text)
{
  "args": {
    "age": "22", 
    "name": "germey"
  }, 
  "headers": {
    "Accept": "*/*", 
    "Accept-Encoding": "gzip, deflate", 
    "Connection": "close", 
    "Host": "httpbin.org", 
    "User-Agent": "python-requests/2.18.4"
  }, 
  "origin": "183.64.61.29", 
  "url": "http://httpbin.org/get?name=germey&age=22"
}
```
或者使用params的方法：
```py
import requests
 
data = {
 'name': 'germey',
 'age': 22
}
response = requests.get("http://httpbin.org/get", params=data)
print(response.text)
```
返回值一样

### 3.3解析json

将返回值已json的形式展示：
```py
import requests
import json
 
response = requests.get("http://httpbin.org/get")
print(type(response.text))
print(response.json())
print(json.loads(response.text))
print(type(response.json()))
```
返回值：
```py
<class 'str'>
{'args': {}, 'headers': {'Accept': '*/*', 'Accept-Encoding': 'gzip, deflate', 'Connection': 'close', 'Host': 'httpbin.org', 'User-Agent': 'python-requests/2.18.4'}, 'origin': '183.64.61.29', 'url': 'http://httpbin.org/get'}
{'args': {}, 'headers': {'Accept': '*/*', 'Accept-Encoding': 'gzip, deflate', 'Connection': 'close', 'Host': 'httpbin.org', 'User-Agent': 'python-requests/2.18.4'}, 'origin': '183.64.61.29', 'url': 'http://httpbin.org/get'}
<class 'dict'>
```
### 3.4获取二进制数据

记住返回值.content就ok了
```py
import requests
 
response = requests.get("https://github.com/favicon.ico")
print(type(response.text), type(response.content))
print(response.text)
print(response.content)
```
返回值为二进制不必再进行展示，

### 3.5添加headers

有些网站访问时必须带有浏览器等信息，如果不传入headers就会报错，如下
```py
import requests
 
response = requests.get("发现 - 知乎")
print(response.text)
```
返回值：
```py
<html><body><h1>500 Server Error</h1>
An internal server error occured.
</body></html>
```
当传入headers时：
```py
import requests
 
headers = {
 'User-Agent': 'Mozilla/5.0 (Macintosh; Intel Mac OS X 10_11_4) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/52.0.2743.116 Safari/537.36'
}
response = requests.get("发现 - 知乎", headers=headers)
print(response.text)
```
成功返回网页源代码不做展示

### 3.6基本POST请求
```py
import requests
 
data = {'name': 'germey', 'age': '22'}
response = requests.post("http://httpbin.org/post", data=data)
print(response.text)
返回：

{
  "args": {}, 
  "data": "", 
  "files": {}, 
  "form": {
    "age": "22", 
    "name": "germey"
  }, 
  "headers": {
    "Accept": "*/*", 
    "Accept-Encoding": "gzip, deflate", 
    "Connection": "close", 
    "Content-Length": "18", 
    "Content-Type": "application/x-www-form-urlencoded", 
    "Host": "httpbin.org", 
    "User-Agent": "python-requests/2.18.4"
  }, 
  "json": null, 
  "origin": "183.64.61.29", 
  "url": "http://httpbin.org/post"
}
```
## 4.响应

### 4.1response属性
```py
import requests
 
response = requests.get('简书 - 创作你的创作')
print(type(response.status_code), response.status_code)
print(type(response.headers), response.headers)
print(type(response.cookies), response.cookies)
print(type(response.url), response.url)
print(type(response.history), response.history)
return：

<class 'int'> 200
<class 'requests.structures.CaseInsensitiveDict'> {'Date': 'Thu, 01 Feb 2018 20:47:08 GMT', 'Server': 'Tengine', 'Content-Type': 'text/html; charset=utf-8', 'Transfer-Encoding': 'chunked', 'X-Frame-Options': 'DENY', 'X-XSS-Protection': '1; mode=block', 'X-Content-Type-Options': 'nosniff', 'ETag': 'W/"9f70e869e7cce214b6e9d90f4ceaa53d"', 'Cache-Control': 'max-age=0, private, must-revalidate', 'Set-Cookie': 'locale=zh-CN; path=/', 'X-Request-Id': '366f4cba-8414-4841-bfe2-792aeb8cf302', 'X-Runtime': '0.008350', 'Content-Encoding': 'gzip', 'X-Via': '1.1 gjf22:8 (Cdn Cache Server V2.0), 1.1 PSzqstdx2ps251:10 (Cdn Cache Server V2.0)', 'Connection': 'keep-alive'}
<class 'requests.cookies.RequestsCookieJar'> <RequestsCookieJar[<Cookie locale=zh-CN for 简书 - 创作你的创作>]>
<class 'str'> 简书 - 创作你的创作
<class 'list'> [<Response [301]>]
```
状态码判断：常见的网页状态码：
```py
100: ('continue',),
101: ('switching_protocols',),
102: ('processing',),
103: ('checkpoint',),
122: ('uri_too_long', 'request_uri_too_long'),
200: ('ok', 'okay', 'all_ok', 'all_okay', 'all_good', '\\o/', '✓'),
201: ('created',),
202: ('accepted',),
203: ('non_authoritative_info', 'non_authoritative_information'),
204: ('no_content',),
205: ('reset_content', 'reset'),
206: ('partial_content', 'partial'),
207: ('multi_status', 'multiple_status', 'multi_stati', 'multiple_stati'),
208: ('already_reported',),
226: ('im_used',),
 
# Redirection.
300: ('multiple_choices',),
301: ('moved_permanently', 'moved', '\\o-'),
302: ('found',),
303: ('see_other', 'other'),
304: ('not_modified',),
305: ('use_proxy',),
306: ('switch_proxy',),
307: ('temporary_redirect', 'temporary_moved', 'temporary'),
308: ('permanent_redirect',
 'resume_incomplete', 'resume',), # These 2 to be removed in 3.0
 
# Client Error.
400: ('bad_request', 'bad'),
401: ('unauthorized',),
402: ('payment_required', 'payment'),
403: ('forbidden',),
404: ('not_found', '-o-'),
405: ('method_not_allowed', 'not_allowed'),
406: ('not_acceptable',),
407: ('proxy_authentication_required', 'proxy_auth', 'proxy_authentication'),
408: ('request_timeout', 'timeout'),
409: ('conflict',),
410: ('gone',),
411: ('length_required',),
412: ('precondition_failed', 'precondition'),
413: ('request_entity_too_large',),
414: ('request_uri_too_large',),
415: ('unsupported_media_type', 'unsupported_media', 'media_type'),
416: ('requested_range_not_satisfiable', 'requested_range', 'range_not_satisfiable'),
417: ('expectation_failed',),
418: ('im_a_teapot', 'teapot', 'i_am_a_teapot'),
421: ('misdirected_request',),
422: ('unprocessable_entity', 'unprocessable'),
423: ('locked',),
424: ('failed_dependency', 'dependency'),
425: ('unordered_collection', 'unordered'),
426: ('upgrade_required', 'upgrade'),
428: ('precondition_required', 'precondition'),
429: ('too_many_requests', 'too_many'),
431: ('header_fields_too_large', 'fields_too_large'),
444: ('no_response', 'none'),
449: ('retry_with', 'retry'),
450: ('blocked_by_windows_parental_controls', 'parental_controls'),
451: ('unavailable_for_legal_reasons', 'legal_reasons'),
499: ('client_closed_request',),
 
# Server Error.
500: ('internal_server_error', 'server_error', '/o\\', '✗'),
501: ('not_implemented',),
502: ('bad_gateway',),
503: ('service_unavailable', 'unavailable'),
504: ('gateway_timeout',),
505: ('http_version_not_supported', 'http_version'),
506: ('variant_also_negotiates',),
507: ('insufficient_storage',),
509: ('bandwidth_limit_exceeded', 'bandwidth'),
510: ('not_extended',),
511: ('network_authentication_required', 'network_auth', 'network_authentication'),
```
## 5.高级操作

### 5.1文件上传

使用 Requests 模块，上传文件也是如此简单的，文件的类型会自动进行处理：

实例：
```py
import requests
 
files = {'file': open('cookie.txt', 'rb')}
response = requests.post("http://httpbin.org/post", files=files)
print(response.text)
```
这是通过测试网站做的一个测试，返回值如下：
```py
{
  "args": {}, 
  "data": "", 
  "files": {
    "file": "#LWP-Cookies-2.0\r\nSet-Cookie3: BAIDUID=\"D2B4E137DE67E271D87F03A8A15DC459:FG=1\"; path=\"/\"; domain=\".baidu.com\"; path_spec; domain_dot; expires=\"2086-02-13 11:15:12Z\"; version=0\r\nSet-Cookie3: BIDUPSID=D2B4E137DE67E271D87F03A8A15DC459; path=\"/\"; domain=\".baidu.com\"; path_spec; domain_dot; expires=\"2086-02-13 11:15:12Z\"; version=0\r\nSet-Cookie3: H_PS_PSSID=25641_1465_21087_17001_22159; path=\"/\"; domain=\".baidu.com\"; path_spec; domain_dot; discard; version=0\r\nSet-Cookie3: PSTM=1516953672; path=\"/\"; domain=\".baidu.com\"; path_spec; domain_dot; expires=\"2086-02-13 11:15:12Z\"; version=0\r\nSet-Cookie3: BDSVRTM=0; path=\"/\"; domain=\"百度一下，你就知道\"; path_spec; discard; version=0\r\nSet-Cookie3: BD_HOME=0; path=\"/\"; domain=\"百度一下，你就知道\"; path_spec; discard; version=0\r\n"
  }, 
  "form": {}, 
  "headers": {
    "Accept": "*/*", 
    "Accept-Encoding": "gzip, deflate", 
    "Connection": "close", 
    "Content-Length": "909", 
    "Content-Type": "multipart/form-data; boundary=84835f570cfa44da8f4a062b097cad49", 
    "Host": "httpbin.org", 
    "User-Agent": "python-requests/2.18.4"
  }, 
  "json": null, 
  "origin": "183.64.61.29", 
  "url": "http://httpbin.org/post"
}{
  "args": {}, 
  "data": "", 
  "files": {
    "file": "#LWP-Cookies-2.0\r\nSet-Cookie3: BAIDUID=\"D2B4E137DE67E271D87F03A8A15DC459:FG=1\"; path=\"/\"; domain=\".baidu.com\"; path_spec; domain_dot; expires=\"2086-02-13 11:15:12Z\"; version=0\r\nSet-Cookie3: BIDUPSID=D2B4E137DE67E271D87F03A8A15DC459; path=\"/\"; domain=\".baidu.com\"; path_spec; domain_dot; expires=\"2086-02-13 11:15:12Z\"; version=0\r\nSet-Cookie3: H_PS_PSSID=25641_1465_21087_17001_22159; path=\"/\"; domain=\".baidu.com\"; path_spec; domain_dot; discard; version=0\r\nSet-Cookie3: PSTM=1516953672; path=\"/\"; domain=\".baidu.com\"; path_spec; domain_dot; expires=\"2086-02-13 11:15:12Z\"; version=0\r\nSet-Cookie3: BDSVRTM=0; path=\"/\"; domain=\"百度一下，你就知道\"; path_spec; discard; version=0\r\nSet-Cookie3: BD_HOME=0; path=\"/\"; domain=\"百度一下，你就知道\"; path_spec; discard; version=0\r\n"
  }, 
  "form": {}, 
  "headers": {
    "Accept": "*/*", 
    "Accept-Encoding": "gzip, deflate", 
    "Connection": "close", 
    "Content-Length": "909", 
    "Content-Type": "multipart/form-data; boundary=84835f570cfa44da8f4a062b097cad49", 
    "Host": "httpbin.org", 
    "User-Agent": "python-requests/2.18.4"
  }, 
  "json": null, 
  "origin": "183.64.61.29", 
  "url": "http://httpbin.org/post"
}
```
### 5.2获取cookie

当需要cookie时，直接调用response.cookie:(response为请求后的返回值)
```py
import requests
 
response = requests.get("百度一下，你就知道")
print(response.cookies)
for key, value in response.cookies.items():
 print(key + '=' + value)
```
输出结果：
```py
<RequestsCookieJar[<Cookie BDORZ=27315 for .baidu.com/>]>
BDORZ=27315
```
### 5.3会话维持、模拟登陆

如果某个响应中包含一些Cookie，你可以快速访问它们：
```py
import requests
 
r = requests.get('http://www.google.com.hk/')
print(r.cookies['NID'])
print(tuple(r.cookies))
```
要想发送你的cookies到服务器，可以使用 cookies 参数：
```py
import requests
 
url = 'http://httpbin.org/cookies'
cookies = {'testCookies_1': 'Hello_Python3', 'testCookies_2': 'Hello_Requests'}
# 在Cookie Version 0中规定空格、方括号、圆括号、等于号、逗号、双引号、斜杠、问号、@，冒号，分号等特殊符号都不能作为Cookie的内容。
r = requests.get(url, cookies=cookies)
print(r.json())
```
### 5.4证书验证

因为12306有一个错误证书，我们那它的网站做测试会出现下面的情况，证书不是官方证书，浏览器会识别出一个错误
```py
import requests
 
response = requests.get('中国铁路12306')
print(response.status_code)
```
返回值：

怎么正常进入这样的网站了，代码如下：
```py
import requests
from requests.packages import urllib3
urllib3.disable_warnings()
response = requests.get('中国铁路12306', verify=False)
print(response.status_code)
将verify设置位False即可，返回的状态码为200
```
urllib3.disable_warnings()这条命令主要用于消除警告信息

### 5.5代理设置

在进行爬虫爬取时，有时候爬虫会被服务器给屏蔽掉，这时采用的方法主要有降低访问时间，通过代理ip访问，如下：
```py
import requests
 
proxies = {
 "http": "http://127.0.0.1:9743",
 "https": "https://127.0.0.1:9743",
}
 
response = requests.get("https://www.taobao.com", proxies=proxies)
print(response.status_code)
```
ip可以从网上抓取，或者某宝购买

如果代理需要设置账户名和密码,只需要将字典更改为如下：
```py
proxies = {
"http":"http://user:password@127.0.0.1:9999"
}
如果你的代理是通过sokces这种方式则需要pip install "requests[socks]"
proxies= {
"http":"socks5://127.0.0.1:9999",
"https":"sockes5://127.0.0.1:8888"
}
```

### 5.6超时设置

访问有些网站时可能会超时，这时设置好timeout就可以解决这个问题
```py
import requests
from requests.exceptions import ReadTimeout
try:
 response = requests.get("http://httpbin.org/get", timeout = 0.5)
 print(response.status_code)
except ReadTimeout:
 print('Timeout')
```
正常访问，状态吗返回200

### 5.7认证设置

如果碰到需要认证的网站可以通过requests.auth模块实现
```py
import requests
 
from requests.auth import HTTPBasicAuth
response = requests.get("http://120.27.34.24:9001/",auth=HTTPBasicAuth("user","123"))
print(response.status_code)
```
当然这里还有一种方式
```py
import requests
 
response = requests.get("http://120.27.34.24:9001/",auth=("user","123"))
print(response.status_code)
```
### 5.8异常处理

遇到网络问题（如：DNS查询失败、拒绝连接等）时，Requests会抛出一个ConnectionError 异常。

遇到罕见的无效HTTP响应时，Requests则会抛出一个 HTTPError 异常。

若请求超时，则抛出一个 Timeout 异常。

若请求超过了设定的最大重定向次数，则会抛出一个 TooManyRedirects 异常。

所有Requests显式抛出的异常都继承自 requests.exceptions.RequestException 。