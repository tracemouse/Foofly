## Foofly ##

[English](README.md) | 中文版

Foofly 是一款Foobar2000插件，使用此插件可以让你远程使用手机控制Foobar2000。

Foofly 分为plugin和App两部分，plugin采用C#编写，App采用ionic4 + AngularJs编写，UI统一采用ionic4的iOS样式，在Safari/Chrome/iOS App/Android App上面呈现的效果完全一致。

- [安装方法](#安装方法)  
  [Plugin 安装方法](#Plugin-安装方法)  
  [App 安装方法](#App-安装方法)   

- [使用方法](#使用方法)   
  [Plugin 使用方法](#Plugin-使用方法)  
  [App 使用方法](#App-使用方法)   

- [多语言支持](#多语言支持)

- [已知的问题](#已知的问题)

- [鸣谢](#鸣谢)

- [联系作者](#联系作者)

- [捐助](#捐助)

## 安装方法 ##

### Plugin 安装方法 ###

从 [release](https://github.com/tracemouse/Foofly/releases) 页面下载最新版本的压缩文件，把解压之后得到的所有文件和子目录拷贝覆盖到Foobar2000 安装目录。 拷贝之前请关闭Foobar2000主程序，拷贝完成后重新打开Foobar2000程序，插件会自动生效。  

由于作者对C++不太熟悉，而foobar2000的原生插件必须使用C++编写，所以只好采用了比较笨的办法，就是用C#写了一个程序叫 foofly-proxy.exe，你只需要双击运行这个程序就行了，这个程序会通过foobar2000的foo_httpcontrol插件来控制foobar2000.  foofly-proxy.exe运行时是默认最小化到任务栏的，你可以通过双击任务栏的图标打开设置窗口，你可以保持foofly-proxy.exe一直开着，当手机端连接时foofly-proxy会自动打开foobar2000主程序。

![plugin-install-1](https://tracemouse.github.io/Foofly/assets/img/plugin-install.jpg)

### App 安装方法 ###

#### iPhone WebApp ####

iOS从**11.3**开始支持PWA(Progress Web Application)，所以你可以通过添加PWA的方式让Foofly变成一个WebApp。  
 
1) 在iPhone的Safari浏览器里面打开下面这个url:

URL  | 说明
 ---- | -----  
http://ip:port/  |  在Foofly-proxy设置窗口可以看见这个url, 只有当Foofly-proxy程序运行时这个URL才能访问

2) 点击Safari下面的`分享按钮`，然后点击`添加到主屏幕`

3) 上面第二步完成之后在主屏幕上就可以看见Foofly图标，点击打开它，没错，看起来和一个普通的iPhone Native App一模一样，enjoy it

![iphone-1](https://tracemouse.github.io/Foobar2000Fly/docs/iphone-1.png)
![iphone-2](https://tracemouse.github.io/Foobar2000Fly/docs/iphone-2.png)
![iphone-3](https://tracemouse.github.io/Foobar2000Fly/docs/iphone-3.png)
![iphone-4](https://tracemouse.github.io/Foobar2000Fly/docs/iphone-4.png)

#### Android WebApp ####

安装步骤方法和上面的iPhone WebApp类似，不过要换成用Google Chrome浏览器来操作。你的安桌手机可能会提示你没有权限，需要在手机设置中放开Chrome添加主屏幕图标的权限，具体设置方法不同品牌手机可能稍有不同。不要试图在你手机品牌自己提供的浏览器上寻找这个功能，一般是没有的，所以需要先安装Google Chrome浏览器.  

另外，Chrome可能会提醒你http是非安全连接，这个忽略就行了，不用理会，因为Foofly没有采用SSL。


## 使用方法 ##

### Plugin 使用方法 ###

foo_httpcontrol的设置在foobar2000的设置面板中，建议保留默认设置不要改动。

Foofly-proxy的设置可以通过双击任务栏图标打开设置窗口，建议都按照默认设置就行了，需要做什么改动。

![plugin-1](https://tracemouse.github.io/Foofly/assets/img/foo_httpcontrol.jpg)
![plugin-2](https://tracemouse.github.io/Foofly/assets/img/foofly-proxy.jpg)

如果你的电脑网卡设置的是DHCP自动获取IP地址，IP地址可能会不固定，这样会造成你使用Foofly App时可能经常需要输入新的IP地址，所以最好把网卡设置为固定IP。设置固定IP有两个方法：  
a) 直接在网卡属性里面把IPV4的地址固定（设置之前先查看DHCP SERVER给你分配的参数，完全设置成一样的就行了)   
b) 在路由器上的DHCP设置里把你的网卡MAC address设为静态分配，也就是分配一个固定的IP地址永不过期。具体设置方法不同的路由器稍有区别，但是大同小异。  

建议采用方法b， 不会的话请百度或者Google。


### App 使用方法 ###
和普通的手机APP一样操作就行了。


- **播放**    

![tab1](https://tracemouse.github.io/Foofly/assets/img/tab1.jpg)


- **正在播放的歌曲**    
播放进度条可以点击控制播放进度，不可以拖动控制  
专辑封面在歌曲播放时默认会旋转，可在设置页面关闭动画  
`专辑封面建议全部使用正方形图片，否则拉伸后会变形，建议的分辨率是500x500`

![playing](https://tracemouse.github.io/Foofly/assets/img/playing.jpg)


- **曲库**    
建议在foobar2000中创建一个“全部歌曲”的播放列表，然后把音乐库中的所有音乐全部添加进去，然后把APP的设置页面把曲库对应的播放列表指向“全部歌曲”。

获取远程音乐库数据时是一次性把远程的所有歌曲全部获取过来，然后在本地进行排序和分类，如果你的音乐库的歌曲数量非常大，可能需要一点时间。作者的曲库中有4000多首歌曲，实测获取速度在可接受的范围。  

![tab2](https://tracemouse.github.io/Foofly/assets/img/tab2.jpg)


- **播放列表**        

![tab3](https://tracemouse.github.io/Foofly/assets/img/tab3.jpg)


- **设置**  
刷新间隔仅对播放页面生效，因为只有播放页面是不停的实时刷新的，默认值是1000毫秒，不建议设置的过大或者过小 
右上角的红色按钮点击之后可以远程控制你的电脑关闭屏幕和关机，关机要小心点，特别是立即关机一旦点击你的电脑马上就会关机

![tab4](https://tracemouse.github.io/Foofly/assets/img/tab4.jpg)


## 多语言支持 ##

Foofly目前支持简体中文、繁体中文和英文三种语言，如果你有兴趣翻译为其他语言，请找到下面这个JSON文件，翻译之后记得发一份给我整合到下一个版本。

[`assets\i18n\en.json`](https://tracemouse.github.io/Foofly/assets/i18n/en.json)

## 已知的问题 ##

- **WebApp 缓存问题**  
WebApp(PWA) 安装模式并没有采用service worker + cache storage，所以依赖的浏览器自动缓存，从目前测试结果来看，Google PWA的效果不错，iOS PWA的离线效果不太好，服务器端离线之后再连线时有时无法从服务器获取版本，所以对于iPhone PWA用户，如果遇到PWA的使用问题，可能通过以下方法解决：

   - 在主屏幕上删除Foofly，然后在iPhone 设置 -> Safari 下面清掉Safari 缓存，然后重新添加WebApp


## 鸣谢 ##

- [foobar2000](http://www.foobar2000.org/) 
- [foo_httpcontrol](http://wiki.hydrogenaud.io/index.php?title=Foobar2000:Components_0.9/HTTP_Control_(foo_httpcontrol))
- [websock-sharp](https://github.com/sta/websocket-sharp)
- [Newtonsoft.Json](https://github.com/JamesNK/Newtonsoft.Json)


## 联系作者 ##

可以给我发电子邮件，但是我不一定会一一回复。  

<tracemouse@163.com>

## 捐助 ##

如果你认可作者的劳动，你可以选择下面任意一种方式(paypal/微信/支付宝)捐助。

**Paypal:**  

[`paypal.me/Yangjun78`](https://paypal.me/Yangjun78)

**微信**

![alipay-wechat](https://tracemouse.github.io/Foofly/assets/img/wechat.png)

**支付宝**

![alipay-wechat](https://tracemouse.github.io/Foofly/assets/img/alipay.png)