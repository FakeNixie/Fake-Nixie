# Fake-Nixie
Fake Nixie erlojuak bost pantaila erabiltzen ditu ordua modu paregabean erakusteko Nixie balbula tradizionalak erabili ordez. Horietako Bi pantailek orduak adierazten dituzte, beste bi minutuak, eta pantaila gehigarri batek bisualki bereizten ditu orduak eta minutuak 2 puntu karakteristikoak erakutsiz, efektu bisual erakargarri bat sortuz.

# Eskertzak

Hau guztia lortzeko hainbat lekutatik atera dugu informazioa, baino gehien bat Owenderew-k egindako github-etik atera dugu:

Owendrew github-a  https://github.com/owendrew/fakeNixie/blob/main/README.md

ESP32ak eta pantailak imagenak erakusteko irudi konbertidorea: https://marlinfw.org/tools/rgb565/converter.html


# Materiala

- [ESP32](https://www.amazon.es/AZDelivery-NodeMCU-ESP-WROOM-32-Tablero-Desarrollo/dp/B071P98VTG/ref=sr_1_1_sspa?adgrpid=130187931296&dib=eyJ2IjoiMSJ9.FnOg5GKRhrrxaQ7ATiBcA_LRJBevbhQHN_ckx39nr2GE1vAHDkrTg2K0F6L4EZ1LDR5ulupm2BGsVBemtBBxtBJtA4KnUVqCC09FDXS-gbGhcdpYiD2zai0C6kqTskkHW9Dkjcd2P2ObUrdWaqmIfJ3CB9N4p-FVsj2NRc5MxrYsDWNTtNcTUZ_AQUUiT178_erb_uJkPwxVmQoU1nQEP_7kNDc3r3BlKzwOSwxwtS09_ixt866iLZgk1NMmjxYP4K7QLp8NowRniDJdWJ7-TLsMahd7DokhrClchdVg8jQ.Vlww67knfSUz1B23Z8c-kOPZcMumci76yrW73qmi6TY&dib_tag=se&hvadid=601382588630&hvdev=c&hvlocphy=9221446&hvnetw=g&hvqmt=e&hvrand=13429030853186878710&hvtargid=kwd-895177109478&hydadcr=19375_2264602&keywords=esp32%2Bcomprar&qid=1739867958&sr=8-1-spons&sp_csd=d2lkZ2V0TmFtZT1zcF9hdGY&th=1) (unitate 1)

- ["ST7789 1.14 inch TFT LCD IPS"](https://www.amazon.es/Fiorky-135x240-Pantalla-pantalla-Interfaz/dp/B0D3C7NP6Z?th=1) (5 unitate)

- [2x4 pines macho de ángulo recto](https://www.amazon.es/LON0167-DC3-8PL-conector-Winkelstecker-IDC-Box-Header/dp/B09FF1Y3KF) (6 unitate)

- ["FPC FFC Cable 8 PIN 0.5mm pitch konektorea"](https://www.amazon.es/dp/B09XMF3QMP/ref=sspa_dk_detail_5?psc=1&pd_rd_i=B09XMF3QMP&pd_rd_w=qWHnr&content-id=amzn1.sym.d9fd07ad-95b5-4079-8602-de55e6918bc7&pf_rd_p=d9fd07ad-95b5-4079-8602-de55e6918bc7&pf_rd_r=QNB533FY2HWJMTRD2XHX&pd_rd_wg=N7F1a&pd_rd_r=8fffe5b7-ce0b-40e3-97cc-32bcca9883dc&s=electronics&sp_csd=d2lkZ2V0TmFtZT1zcF9kZXRhaWw) (5 unitate)

- [1x10 male header pins](https://es.farnell.com/wurth-elektronik/61301011121/macho-2-54-mm-pin-10-v-as/dp/1841229?srsltid=AfmBOoohEvH4zGba-wI5Lr7jzcLDaATOVk_Ns8ygC9LR6aUuoJAROVMa) (4 unitate)

- [2x4 female header sockets (2,54mm pitch)](https://es.aliexpress.com/item/1005004372795403.html?gps-id=platformRecommendH5ForSpider&pvid=5058ee50-df86-4ca0-9f69-127fb1ced0d3&_t=gps-id:platformRecommendH5ForSpider,pvid:5058ee50-df86-4ca0-9f69-127fb1ced0d3,tpp_buckets:668%232846%238108%231977&pdp_npi=4%40dis%21EUR%210.82%210.82%21%21%210.88%210.88%21%40210318c317212735906453162efde5%21-1%21rec%21ES%21%21AB) (5 unitate)

- [Micro USB PSU kablea](https://tienda.bricogeek.com/cables/1471-cable-usb-micro-b-corto-85cm.html?_gl=1*1uiv34e*_up*MQ..&gclid=Cj0KCQiAgJa6BhCOARIsAMiL7V_qXfVjoZB_y0qWb7v6-4lT60nsX6upKu_WkkXCQMTkMG6tsh5sPBsaAuLSEALw_wcB) (unitate 1)

- [Pantailak tapatzeko kupulak](https://www.temu.com/es/kuiper/un9.html?subj=coupon-un&_bg_fs=1&_p_jump_id=895&_x_vst_scene=adg&goods_id=601099634148124&sku_id=17592652742935&adg_ctx=a-3e6b91c4~c-48926975~f-9c72e498&_x_ads_sub_channel=shopping&_p_rfs=1&_x_ns_prz_type=-1&_x_ns_sku_id=17592652742935&_x_ns_gid=601099634148124&mrk_rec=1&_x_ads_channel=google&_x_gmc_account=742367270&_x_login_type=Google&_x_ads_account=4438999299&_x_ads_set=20564658662&_x_ads_id=159325185088&_x_ads_creative_id=674377931467&_x_ns_source=g&_x_ns_gclid=Cj0KCQiA_NC9BhCkARIsABSnSTZ2uz9OS2V6xeKOOVSZX851rEgGwYTbiT5zgC-U0reCbOs-mjrWVcIaAmQKEALw_wcB&_x_ns_placement=&_x_ns_match_type=&_x_ns_ad_position=&_x_ns_product_id=17592652742935&_x_ns_target=&_x_ns_devicemodel=&_x_ns_wbraid=Cj4KCAiA2cu9BhBTEi4Aad609Cg3Z2e3BUHVpfq7W9BRAmuR1IYoEjFvkv3kcpYrJfTKDuFiKHyj9lZ-GgIasA&_x_ns_gbraid=0AAAAAo4mICE07tS68k-xC_ac7Fwz8A_eg&_x_ns_targetid=pla-2091589665000&gad_source=1&gclid=Cj0KCQiA_NC9BhCkARIsABSnSTZ2uz9OS2V6xeKOOVSZX851rEgGwYTbiT5zgC-U0reCbOs-mjrWVcIaAmQKEALw_wcB) (5 unitate)

- [Portapilas 2 AA](https://es.rs-online.com/web/p/portapilas/1854604?cm_mmc=ES-PLA-DS3A-_-google-_-CSS_ES_ES_Pmax_RS+PRO-_--_-1854604&matchtype=&&gad_source=1&gclid=Cj0KCQiA_NC9BhCkARIsABSnSTYtC_CpVrhS9ebgAUzcrLOFOdoRWmweOqMXp6LaKxzXJyts6neNAzcaAllREALw_wcB&gclsrc=aw.ds)  (2 unitate)

# [Pantailen Muntaketa](https://github.com/FakeNixie/Fake-Nixie/tree/main/Dokumentazioa/Eskemak)

![image](https://github.com/user-attachments/assets/aff479ca-8f9e-4b8a-a7ba-7f6454a1d367)

Hasteko pataillak konektatzen hasi ginen jakiteko nola funtzionatzen zuten. Goian ikusi dezakezue guk egindako eskema bat, bertan  nola konektatu eta ze pinetara eraman genituen pantailen kable bakoitza erakusten du. Eskema hori egin guk egin genuen ez genuelako datasheet argirik aurkitzen.

# [Konexio Esquema](https://github.com/FakeNixie/Fake-Nixie/tree/main/Dokumentazioa/Eskemak)

![image](https://github.com/user-attachments/assets/3114085d-0b17-4022-b7e7-bbfad8fa33f3)

Goiko irudian ikusi dezakezue guk egindako konexio esquema. Bertan ESP32-ra nola konektatu dituegun pantailak ikusi dezakezue. 5 pantailak pin berdinetara dijoazte konektatutak aldatzen den pin bakarra 8.pina da. Pin hoenk, informazioa eramaten duena, hau da, pantaiak rekusten duen irudi bakoitzak behar duen informazioa pin horretatik sartuko da. konexio guztia irudikatzeko eta eskema egiteko Proteus aplikazioa erabili dugu.

# [PCB Diseinua](https://github.com/FakeNixie/Fake-Nixie/tree/main/Dokumentazioa/PCB_Artxiboak)

![image](https://github.com/user-attachments/assets/d187a726-ffe8-499f-8b25-47bf2ef6fd76) ![image](https://github.com/user-attachments/assets/52a45151-aee1-48ad-89c3-de00aa218f60)

Gure hurrengo pausua esquemaren pcb-a egitea izan zen. PCB-a egin genuen ez kablerik erabiltzeko eta dena modu ordenatu batean gelditzeko, horrela interferentziak saiestuko genituen eta konexio arazoak ekidituko genituen. Hau egiteko ere Proteus aplikazioa erabili dugu.

# [Kodigoa](https://github.com/FakeNixie/Fake-Nixie/tree/main/Dokumentazioa/Arduino)
![image](https://github.com/user-attachments/assets/7f8ca756-9e86-4262-abbb-3f68a19a38e5)

Gure kodea hasiera batean ordua erakusten zuen bakarrik zuk emandako wifiaren bidez, zure ordu-eremuko ordua hartzen du eta pantailetan erakusten du gainera ordua WiFi bidez hartzen duenez erlojua beti orduan egongo da. Gero, aplikazioa baten bitertez bi estilo ezberdin ditugu sartutak kodigoan eta aplikazioaren bidez kodigoan sortutako web era sartu eta estiloz aldatu dezakezu. Gainera web-aren bitartez beste erloju normal batzuen bezala cronometro bat martxan jarri dezakezu eta zure erlojuan bistaratu.

# Elikadura

![image](https://github.com/user-attachments/assets/74cdc955-89e4-493e-a47e-16d6847cf465)

Muntaiekin bukatzeko elikatzeko modu bat egin genuen gure proiektua ez egoteko ordenagailura konektatuta donebora guztian, hau lortzeko bi portapilas hartu genituen eta 4 pilako portapilas batean bihurtu genuen. Hoenk 6V-ko tentsioa hematen dio ESP32ari eta piztuta egon daiteke edozien tokitan ordenagailuara konektatuta eduki gabe.
# [Aplikazioa](https://sites.google.com/d/18WV8YJMY_IQXxaLhMr7m3VHMnpldd2XD/p/1QMYbZgTesbHUEUoDU0BAqGxNVekRPJkL/edit)
![image](https://github.com/user-attachments/assets/5a6a522a-c2a7-49e2-8022-3eded205d2f2)

Bukatzeko aplikazioa bat egin genuen. Aplikazioa ez da aplkazio normal bat, sites bat da ip helbide batera eramaten dizuena. Honekin lortzen dugu aplikazio moduko bat edukitzera erlojua konfiguratzeko web-era bideratzen diguena. Aplikazio honetan ordua ikusi dezakegu eta estiloz aldatu deazkegu erlojua gure gustura gelditzeko, kronometro bat hasi dezakezu ere botoi batekin eta gelditu.
