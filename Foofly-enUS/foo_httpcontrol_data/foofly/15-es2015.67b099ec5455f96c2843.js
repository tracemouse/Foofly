(window.webpackJsonp=window.webpackJsonp||[]).push([[15],{"f+ep":function(n,l,t){"use strict";t.r(l);var e=t("8Y7J"),i=t("mrSG"),o=t("ZZ/e"),u=t("aR35"),r=t("avBb"),s=t("4EUy");class a{constructor(n,l,t,e,i,o,r){this.navCtrl=n,this.translateService=l,this.alertController=t,this.myDBService=e,this.activeRoute=i,this.myHttpService=o,this.loadingController=r,this.inputIp=u.a.settings.ip,this.inputPort=u.a.settings.port,this.inputPassword=u.a.settings.password,this.loginDisabled=!1,this.loadingDuration=6e4,this.bandIps=["foofly.tracemouse.top"],this.activeRoute.queryParams.subscribe(n=>{this.from=n.from})}ngOnInit(){}ionViewWillEnter(){return i.b(this,void 0,void 0,function*(){this.inputIp=u.a.settings.ip,this.inputPort=u.a.settings.port,this.inputPassword=u.a.settings.password,this.bandIps.indexOf(this.inputIp)>=0&&(this.inputIp="",this.inputPort=""),"exit"!=this.from&&this.myHttpService.GetState().then(n=>{this.loginDisabled=!1,this.navCtrl.navigateForward("/tabs/tab1")},n=>{this.loginDisabled=!1})})}initLoading(){return i.b(this,void 0,void 0,function*(){this.loading=yield this.loadingController.create({duration:this.loadingDuration,message:"",translucent:!0,cssClass:"custom-class custom-loading"})})}logIn(){return i.b(this,void 0,void 0,function*(){u.a.settings.ip=this.inputIp,u.a.settings.port=this.inputPort,u.a.settings.rootUrl=u.a.settings.protocol+"//"+u.a.settings.ip+":"+u.a.settings.port,this.myDBService.saveSettingsData(),this.loginDisabled=!0,this.testHttp()})}getIp(n){this.inputIp=n.value}getPort(n){this.inputPort=n.value}getPassword(n){this.inputPassword=n.value}testHttp(){return i.b(this,void 0,void 0,function*(){yield this.initLoading(),yield this.loading.present(),this.myHttpService.GetState().then(n=>{this.loginDisabled=!1,this.loading.dismiss(),this.navCtrl.navigateForward("/tabs/tab1")},n=>{this.loginDisabled=!1,this.loading.dismiss(),this.presentConnError()})})}testConn(n){return i.b(this,void 0,void 0,function*(){n=n+u.a.fooflyRoot+"assets/version.js";var l=document.createElement("script");l.setAttribute("id","testConnScript"),l.onload=function(n){document.getElementById("testConnScript").remove();let l=document.getElementById("connCheckbox");l.setAttribute("conn","true"),l.shadowRoot.lastElementChild.click()},l.onerror=function(){document.getElementById("testConnScript").remove();let n=document.getElementById("connCheckbox");n.setAttribute("conn","false"),n.shadowRoot.lastElementChild.click()},l.setAttribute("src",n),document.getElementsByTagName("head")[0].appendChild(l)})}connChanged(n){var l=n.srcElement.getAttribute("conn");this.loginDisabled=!1,"true"==l?"dev"==u.a.env?this.navCtrl.navigateForward("/tabs/tab1"):location.href=u.a.settings.rootUrl+"/foofly/index.html#/tabs/tab1":this.presentConnError()}presentConnError(){return i.b(this,void 0,void 0,function*(){var n;yield this.translateService.get("message").subscribe(l=>{n=l});const l=yield this.alertController.create({header:n.error,subHeader:n["err-conn-fail"],message:n["err-conn-fail-msg"],buttons:[n.ok]});l.onDidDismiss().then(n=>{this.loginDisabled=!1}),yield l.present()})}}class b{}var c=t("pMnS"),d=t("oBZk"),p=t("TSSN"),h=t("s7LF"),g=t("iInd"),m=e.pb({encapsulation:0,styles:[["ion-item[_ngcontent-%COMP%]{padding-right:16px}.header-toolbar[_ngcontent-%COMP%]{--border-width:0!important}.lbl-button[_ngcontent-%COMP%]{margin-top:15px}.button[_ngcontent-%COMP%]{--border-radius:25px!important}.logo[_ngcontent-%COMP%]{width:100px}.logo-box[_ngcontent-%COMP%]{margin:0 auto 15px;text-align:center}"]],data:{}});function v(n){return e.Mb(0,[(n()(),e.rb(0,0,null,null,3,"ion-header",[],null,null,null,d.T,d.j)),e.qb(1,49152,null,0,o.B,[e.h,e.k,e.x],null,null),(n()(),e.rb(2,0,null,0,1,"ion-toolbar",[["class","header-toolbar"],["translucent",""]],null,null,null,d.tb,d.J)),e.qb(3,49152,null,0,o.Bb,[e.h,e.k,e.x],null,null),(n()(),e.rb(4,0,null,null,43,"ion-content",[],null,null,null,d.R,d.h)),e.qb(5,49152,null,0,o.u,[e.h,e.k,e.x],null,null),(n()(),e.rb(6,0,null,0,37,"ion-list",[],null,null,null,d.bb,d.q)),e.qb(7,49152,null,0,o.O,[e.h,e.k,e.x],null,null),(n()(),e.rb(8,0,null,0,1,"div",[["class","logo-box"]],null,null,null,null,null)),(n()(),e.rb(9,0,null,null,0,"img",[["class","logo"],["src","assets/img/logo.jpg"]],null,null,null,null,null)),(n()(),e.rb(10,0,null,0,12,"ion-item",[["class","mb-input"]],null,null,null,d.Y,d.o)),e.qb(11,49152,null,0,o.H,[e.h,e.k,e.x],null,null),(n()(),e.rb(12,0,null,0,6,"ion-label",[["position","floating"]],null,null,null,d.Z,d.p)),e.qb(13,49152,null,0,o.N,[e.h,e.k,e.x],{position:[0,"position"]},null),(n()(),e.Kb(14,0,[""," "])),e.Fb(131072,p.i,[p.j,e.h]),(n()(),e.rb(16,0,null,0,2,"ion-text",[["color","danger"]],null,null,null,d.qb,d.G)),e.qb(17,49152,null,0,o.wb,[e.h,e.k,e.x],{color:[0,"color"]},null),(n()(),e.Kb(-1,0,["*"])),(n()(),e.rb(19,0,null,0,3,"ion-input",[["clearInput",""],["inputmode","ip"],["placeholder","192.168.1.10"],["required","true"],["type","text"]],null,[[null,"ionBlur"],[null,"ionChange"]],function(n,l,t){var i=!0,o=n.component;return"ionBlur"===l&&(i=!1!==e.Db(n,22)._handleBlurEvent(t.target)&&i),"ionChange"===l&&(i=!1!==e.Db(n,22)._handleInputEvent(t.target)&&i),"ionBlur"===l&&(i=!1!==o.getIp(e.Db(n,21))&&i),i},d.X,d.n)),e.Hb(5120,null,h.g,function(n){return[n]},[o.Nb]),e.qb(21,49152,[["ip",4]],0,o.G,[e.h,e.k,e.x],{clearInput:[0,"clearInput"],inputmode:[1,"inputmode"],placeholder:[2,"placeholder"],required:[3,"required"],type:[4,"type"],value:[5,"value"]},null),e.qb(22,16384,null,0,o.Nb,[e.k],null,null),(n()(),e.rb(23,0,null,0,12,"ion-item",[["class","mb-input"]],null,null,null,d.Y,d.o)),e.qb(24,49152,null,0,o.H,[e.h,e.k,e.x],null,null),(n()(),e.rb(25,0,null,0,6,"ion-label",[["position","floating"]],null,null,null,d.Z,d.p)),e.qb(26,49152,null,0,o.N,[e.h,e.k,e.x],{position:[0,"position"]},null),(n()(),e.Kb(27,0,[""," "])),e.Fb(131072,p.i,[p.j,e.h]),(n()(),e.rb(29,0,null,0,2,"ion-text",[["color","danger"]],null,null,null,d.qb,d.G)),e.qb(30,49152,null,0,o.wb,[e.h,e.k,e.x],{color:[0,"color"]},null),(n()(),e.Kb(-1,0,["*"])),(n()(),e.rb(32,0,null,0,3,"ion-input",[["clearInput",""],["inputmode","tel"],["maxlength","5"],["minlength","2"],["pattern","[0-9]*"],["placeholder","8888"],["required","true"],["type","tel"]],null,[[null,"ionBlur"],[null,"ionChange"]],function(n,l,t){var i=!0,o=n.component;return"ionBlur"===l&&(i=!1!==e.Db(n,35)._handleBlurEvent(t.target)&&i),"ionChange"===l&&(i=!1!==e.Db(n,35)._handleInputEvent(t.target)&&i),"ionBlur"===l&&(i=!1!==o.getPort(e.Db(n,34))&&i),i},d.X,d.n)),e.Hb(5120,null,h.g,function(n){return[n]},[o.Nb]),e.qb(34,49152,[["port",4]],0,o.G,[e.h,e.k,e.x],{clearInput:[0,"clearInput"],inputmode:[1,"inputmode"],maxlength:[2,"maxlength"],minlength:[3,"minlength"],pattern:[4,"pattern"],placeholder:[5,"placeholder"],required:[6,"required"],type:[7,"type"],value:[8,"value"]},null),e.qb(35,16384,null,0,o.Nb,[e.k],null,null),(n()(),e.rb(36,0,null,0,7,"ion-item",[["lines","none"]],null,null,null,d.Y,d.o)),e.qb(37,49152,null,0,o.H,[e.h,e.k,e.x],{lines:[0,"lines"]},null),(n()(),e.rb(38,0,null,0,5,"ion-label",[["class","lbl-button"]],null,null,null,d.Z,d.p)),e.qb(39,49152,null,0,o.N,[e.h,e.k,e.x],null,null),(n()(),e.rb(40,0,null,0,3,"ion-button",[["class","button"],["color","danger"],["expand","block"],["size","default"]],null,[[null,"click"]],function(n,l,t){var e=!0;return"click"===l&&(e=!1!==n.component.logIn()&&e),e},d.M,d.c)),e.qb(41,49152,null,0,o.k,[e.h,e.k,e.x],{color:[0,"color"],disabled:[1,"disabled"],expand:[2,"expand"],size:[3,"size"]},null),(n()(),e.Kb(42,0,["",""])),e.Fb(131072,p.i,[p.j,e.h]),(n()(),e.rb(44,0,null,0,3,"ion-checkbox",[["id","connCheckbox"],["style","display:none"]],null,[[null,"ionChange"],[null,"ionBlur"]],function(n,l,t){var i=!0,o=n.component;return"ionBlur"===l&&(i=!1!==e.Db(n,47)._handleBlurEvent(t.target)&&i),"ionChange"===l&&(i=!1!==e.Db(n,47)._handleIonChange(t.target)&&i),"ionChange"===l&&(i=!1!==o.connChanged(t)&&i),i},d.P,d.f)),e.Hb(5120,null,h.g,function(n){return[n]},[o.d]),e.qb(46,49152,null,0,o.r,[e.h,e.k,e.x],null,null),e.qb(47,16384,null,0,o.d,[e.k],null,null)],function(n,l){var t=l.component;n(l,13,0,"floating"),n(l,17,0,"danger"),n(l,21,0,"","ip","192.168.1.10","true","text",e.vb(1,"",t.inputIp,"")),n(l,26,0,"floating"),n(l,30,0,"danger"),n(l,34,0,"","tel","5","2","[0-9]*","8888","true","tel",e.vb(1,"",t.inputPort,"")),n(l,37,0,"none"),n(l,41,0,"danger",e.vb(1,"",t.loginDisabled,""),"block","default")},function(n,l){n(l,14,0,e.Lb(l,14,0,e.Db(l,15).transform("common.ip"))),n(l,27,0,e.Lb(l,27,0,e.Db(l,28).transform("common.port"))),n(l,42,0,e.Lb(l,42,0,e.Db(l,43).transform("login.button-text")))})}function f(n){return e.Mb(0,[(n()(),e.rb(0,0,null,null,1,"app-login",[],null,null,null,v,m)),e.qb(1,114688,null,0,a,[o.Hb,p.j,o.b,r.a,g.a,s.a,o.Fb],null,null)],function(n,l){n(l,1,0)},null)}var x=e.nb("app-login",a,f,{},{},[]),C=t("SVse");t.d(l,"LoginPageModuleNgFactory",function(){return k});var k=e.ob(b,[],function(n){return e.Ab([e.Bb(512,e.j,e.Z,[[8,[c.a,x]],[3,e.j],e.v]),e.Bb(4608,C.l,C.k,[e.s,[2,C.v]]),e.Bb(4608,h.o,h.o,[]),e.Bb(4608,o.c,o.c,[e.x,e.g]),e.Bb(4608,o.Gb,o.Gb,[o.c,e.j,e.p]),e.Bb(4608,o.Kb,o.Kb,[o.c,e.j,e.p]),e.Bb(1073742336,C.b,C.b,[]),e.Bb(1073742336,h.n,h.n,[]),e.Bb(1073742336,h.d,h.d,[]),e.Bb(1073742336,o.Db,o.Db,[]),e.Bb(1073742336,p.g,p.g,[]),e.Bb(1073742336,g.o,g.o,[[2,g.t],[2,g.m]]),e.Bb(1073742336,b,b,[]),e.Bb(1024,g.k,function(){return[[{path:"",component:a}]]},[])])})}}]);