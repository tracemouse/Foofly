## Foofly ##

English | [**中文版**](README-CN.md)

Foofly is a Foobar2000 plugin to enable remote controling of [**Foobar2000**](http://www.foobar2000.org/) by your mobile phone.

Foofly does include two parts : Plugin and App, Plugin was built by C#, App was built by ionic4 + AngularJs. The UI uses iOS style of ionic4 to show the same style on different platforms. 

- [Installation](#Installation)  
  [Plugin Installation](#Plugin-Installation)   
  [App Installation](#App-Installation)   

- [Usage](#Usage)   
  [Plugin Usage](#Plugin-Usage)  
  [App Usage](#App-Usage)   

- [Multiple Languages](#Multiple-Languages)

- [Knows Issues](#Knows-Issues)

- [Thanks](#Thanks)

- [Contact](#Contact)

- [Donate](#Donate)

## Installation ##

### Plugin Installation ###

Get the latest plugin from the [release](https://github.com/tracemouse/Foofly/releases) page,
unzip the zip file and copy the file & folders into the folder of `Foobar2000`.
Please remember to close the Foobar2000 main program, the plugin will be effective when you re-open the Foobar2000 program. 

![plugin-1](https://tracemouse.github.io/Foofly/docs/plugin-1.png)

Now you run the program Foofly-proxy(`Foofly-proxy.exe`), and you can double click the notifyicon to open the main window. 


### App Installation ###

App could be installed as WebApp by Safari(iOS) or Chrome(Android).

#### iPhone WebApp ####

iOS starts to support PWA(Progress Web Application) from **11.3**, please refer to below steps to add Foofly as a WebApp.  
 
1) Open the below url on your iPhone's Safari

URL  | Remark
 ---- | -----  
http://ip:port/  |  Provided by your Foofly-proxy(`Foofly-proxy.exe`), only available when your Foofly-proxy are opening

2) Click the share button and then click the button `Add to main screen`

3) Now you can see the Foofly icon on your main screen, click it like a native app, enjoy it!

![iphone-1](https://tracemouse.github.io/Foofly/docs/iphone-1.png)
![iphone-2](https://tracemouse.github.io/Foofly/docs/iphone-2.png)
![iphone-3](https://tracemouse.github.io/Foofly/docs/iphone-3.png)
![iphone-4](https://tracemouse.github.io/Foofly/docs/iphone-4.png)

#### Android WebApp ####

Use Google Chrome to do it like iPhone Safari above, Chrome may remind you the HTTP is unsafe, please ignore it because Foofly cannot use SSL.

![chrome](https://tracemouse.github.io/Foofly/docs/chrome.png)
![chrome2](https://tracemouse.github.io/Foofly/docs/chrome-2.png)

## Usage ##

### Plugin Usage ###

I don't know how to use C++ to write a native foobar2000 plugin, so I used C# to write a program named Foofly-proxy(`Foofly-proxy.exe`) to call the httpcontorl plugin of foobar2000.

It's enough to run the program Foofly-proxy, and it will auto open the foolbar2000 main program when app connection starts.
If you don't know how to config the settings of Foofly-proxy & httpcontrol, just keep it as default.

The httpcontrol configration file is located at "configuration\foo_httpcontrol.dll.cfg", delete it and re-open the foobar2000 program, the httpcontorl setting will be fallback as default.


![plugin-2](https://tracemouse.github.io/Foofly/docs/plugin-2.png)

### App Usage ###

- **Now Playing List**  

![tab1](https://tracemouse.github.io/Foofly/docs/tab1.png)

- **Now Playing Track**

![playing](https://tracemouse.github.io/Foofly/docs/playing.png)

- **Music Library**  
Suggest to create a playlist named "All tracks" to include all songs of your music library, and then set the source playlist of the music library of Foofly as "All tracks".

![tab2](https://tracemouse.github.io/Foofly/docs/tab2.png)

- **Search**  

![search](https://tracemouse.github.io/Foofly/docs/search.png)

- **Playlist**  

![tab3](https://tracemouse.github.io/Foofly/docs/tab3.png)

- **Settings**

![tab4](https://tracemouse.github.io/Foofly/docs/tab4.png)
 

## Multiple Languages ##

Foofly Fly App now supports 简体中文、繁体中文 and English, if you would translate it as other language, please try to transalte the below JSON file and send back me for next version.

[`assets\i18n\en.json`](https://tracemouse.github.io/Foofly/assets/i18n/en.json)

## Known Issues ##

- **WebApp cache problem**  
iPhone WebApp may eccounter some problems because it depends on auto cache of Safari, if the WebApp doesn't work, you may try the following approaches，  

   - Delete Foofly Fly from main screen, clean the Safari cache, and re-add the WebApp


## Thanks ##

- [foobar2000](http://www.foobar2000.org/) 
- [foo_httpcontrol](http://wiki.hydrogenaud.io/index.php?title=Foobar2000:Components_0.9/HTTP_Control_(foo_httpcontrol))
- [websock-sharp](https://github.com/sta/websocket-sharp)
- [Newtonsoft.Json](https://github.com/JamesNK/Newtonsoft.Json)


## Contact ##

<tracemouse@163.com>

## Donate ##

If you like my project, please donate it to me.

**Paypal:**  

[`paypal.me/Yangjun78`](https://paypal.me/Yangjun78)

**微信**

![alipay-wechat](https://tracemouse.github.io/Foofly/docs/wechat.png)

**支付宝**

![alipay-wechat](https://tracemouse.github.io/Foofly/docs/alipay.png)