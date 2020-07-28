<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE eagle SYSTEM "eagle.dtd">
<eagle version="9.6.2">
<drawing>
<settings>
<setting alwaysvectorfont="no"/>
<setting verticaltext="up"/>
</settings>
<grid distance="0.1" unitdist="inch" unit="inch" style="lines" multiple="1" display="no" altdistance="0.01" altunitdist="inch" altunit="inch"/>
<layers>
<layer number="1" name="Top" color="4" fill="1" visible="no" active="no"/>
<layer number="2" name="Route2" color="16" fill="1" visible="no" active="no"/>
<layer number="3" name="Route3" color="17" fill="1" visible="no" active="no"/>
<layer number="4" name="Route4" color="18" fill="1" visible="no" active="no"/>
<layer number="5" name="Route5" color="19" fill="1" visible="no" active="no"/>
<layer number="6" name="Route6" color="25" fill="1" visible="no" active="no"/>
<layer number="7" name="Route7" color="26" fill="1" visible="no" active="no"/>
<layer number="8" name="Route8" color="27" fill="1" visible="no" active="no"/>
<layer number="9" name="Route9" color="28" fill="1" visible="no" active="no"/>
<layer number="10" name="Route10" color="29" fill="1" visible="no" active="no"/>
<layer number="11" name="Route11" color="30" fill="1" visible="no" active="no"/>
<layer number="12" name="Route12" color="20" fill="1" visible="no" active="no"/>
<layer number="13" name="Route13" color="21" fill="1" visible="no" active="no"/>
<layer number="14" name="Route14" color="22" fill="1" visible="no" active="no"/>
<layer number="15" name="Route15" color="23" fill="1" visible="no" active="no"/>
<layer number="16" name="Bottom" color="1" fill="1" visible="no" active="no"/>
<layer number="17" name="Pads" color="2" fill="1" visible="no" active="no"/>
<layer number="18" name="Vias" color="2" fill="1" visible="no" active="no"/>
<layer number="19" name="Unrouted" color="6" fill="1" visible="no" active="no"/>
<layer number="20" name="Dimension" color="15" fill="1" visible="no" active="no"/>
<layer number="21" name="tPlace" color="7" fill="1" visible="no" active="no"/>
<layer number="22" name="bPlace" color="7" fill="1" visible="no" active="no"/>
<layer number="23" name="tOrigins" color="15" fill="1" visible="no" active="no"/>
<layer number="24" name="bOrigins" color="15" fill="1" visible="no" active="no"/>
<layer number="25" name="tNames" color="7" fill="1" visible="no" active="no"/>
<layer number="26" name="bNames" color="7" fill="1" visible="no" active="no"/>
<layer number="27" name="tValues" color="7" fill="1" visible="no" active="no"/>
<layer number="28" name="bValues" color="7" fill="1" visible="no" active="no"/>
<layer number="29" name="tStop" color="7" fill="3" visible="no" active="no"/>
<layer number="30" name="bStop" color="7" fill="6" visible="no" active="no"/>
<layer number="31" name="tCream" color="7" fill="4" visible="no" active="no"/>
<layer number="32" name="bCream" color="7" fill="5" visible="no" active="no"/>
<layer number="33" name="tFinish" color="6" fill="3" visible="no" active="no"/>
<layer number="34" name="bFinish" color="6" fill="6" visible="no" active="no"/>
<layer number="35" name="tGlue" color="7" fill="4" visible="no" active="no"/>
<layer number="36" name="bGlue" color="7" fill="5" visible="no" active="no"/>
<layer number="37" name="tTest" color="7" fill="1" visible="no" active="no"/>
<layer number="38" name="bTest" color="7" fill="1" visible="no" active="no"/>
<layer number="39" name="tKeepout" color="4" fill="11" visible="no" active="no"/>
<layer number="40" name="bKeepout" color="1" fill="11" visible="no" active="no"/>
<layer number="41" name="tRestrict" color="4" fill="10" visible="no" active="no"/>
<layer number="42" name="bRestrict" color="1" fill="10" visible="no" active="no"/>
<layer number="43" name="vRestrict" color="2" fill="10" visible="no" active="no"/>
<layer number="44" name="Drills" color="7" fill="1" visible="no" active="no"/>
<layer number="45" name="Holes" color="7" fill="1" visible="no" active="no"/>
<layer number="46" name="Milling" color="3" fill="1" visible="no" active="no"/>
<layer number="47" name="Measures" color="7" fill="1" visible="no" active="no"/>
<layer number="48" name="Document" color="7" fill="1" visible="no" active="no"/>
<layer number="49" name="Reference" color="7" fill="1" visible="no" active="no"/>
<layer number="51" name="tDocu" color="7" fill="1" visible="no" active="no"/>
<layer number="52" name="bDocu" color="7" fill="1" visible="no" active="no"/>
<layer number="88" name="SimResults" color="9" fill="1" visible="yes" active="yes"/>
<layer number="89" name="SimProbes" color="9" fill="1" visible="yes" active="yes"/>
<layer number="90" name="Modules" color="5" fill="1" visible="yes" active="yes"/>
<layer number="91" name="Nets" color="2" fill="1" visible="yes" active="yes"/>
<layer number="92" name="Busses" color="1" fill="1" visible="yes" active="yes"/>
<layer number="93" name="Pins" color="2" fill="1" visible="no" active="yes"/>
<layer number="94" name="Symbols" color="4" fill="1" visible="yes" active="yes"/>
<layer number="95" name="Names" color="7" fill="1" visible="yes" active="yes"/>
<layer number="96" name="Values" color="7" fill="1" visible="yes" active="yes"/>
<layer number="97" name="Info" color="7" fill="1" visible="yes" active="yes"/>
<layer number="98" name="Guide" color="6" fill="1" visible="yes" active="yes"/>
</layers>
<schematic xreflabel="%F%N/%S.%C%R" xrefpart="/%S.%C%R">
<libraries>
<library name="ESP32-D0WDQ6">
<packages>
<package name="QFN35P500X500X90-49N">
<rectangle x1="-1.36213125" y1="-1.36213125" x2="1.36" y2="1.36" layer="31"/>
<text x="-3.3503" y="-4.1188" size="1.27583125" layer="27" align="top-left">&gt;VALUE</text>
<text x="-3.340259375" y="4.10646875" size="1.272" layer="25">&gt;NAME</text>
<circle x="-3.755" y="2.275" radius="0.1" width="0.2" layer="21"/>
<circle x="-2.655" y="2.2" radius="0.1" width="0.2" layer="51"/>
<wire x1="3.03" y1="-3.03" x2="-3.03" y2="-3.03" width="0.127" layer="51"/>
<wire x1="3.03" y1="3.03" x2="-3.03" y2="3.03" width="0.127" layer="51"/>
<wire x1="3.03" y1="-3.03" x2="3.03" y2="3.03" width="0.127" layer="51"/>
<wire x1="-3.03" y1="-3.03" x2="-3.03" y2="3.03" width="0.127" layer="51"/>
<wire x1="-3.6" y1="-3.6" x2="3.6" y2="-3.6" width="0.05" layer="39"/>
<wire x1="-3.6" y1="3.6" x2="3.6" y2="3.6" width="0.05" layer="39"/>
<wire x1="-3.6" y1="-3.6" x2="-3.6" y2="3.6" width="0.05" layer="39"/>
<wire x1="3.6" y1="-3.6" x2="3.6" y2="3.6" width="0.05" layer="39"/>
<wire x1="-2.6" y1="3.03" x2="-3.03" y2="3.03" width="0.127" layer="21"/>
<wire x1="-3.03" y1="3.03" x2="-3.03" y2="2.6" width="0.127" layer="21"/>
<wire x1="2.6" y1="-3.03" x2="3.03" y2="-3.03" width="0.127" layer="21"/>
<wire x1="3.03" y1="-3.03" x2="3.03" y2="-2.6" width="0.127" layer="21"/>
<wire x1="2.6" y1="3.03" x2="3.03" y2="3.03" width="0.127" layer="21"/>
<wire x1="3.03" y1="3.03" x2="3.03" y2="2.6" width="0.127" layer="21"/>
<wire x1="-2.6" y1="-3.03" x2="-3.03" y2="-3.03" width="0.127" layer="21"/>
<wire x1="-3.03" y1="-3.03" x2="-3.03" y2="-2.6" width="0.127" layer="21"/>
<smd name="15" x="-1.4" y="-2.945" dx="0.81" dy="0.22" layer="1" roundness="25" rot="R90"/>
<smd name="16" x="-1" y="-2.945" dx="0.81" dy="0.22" layer="1" roundness="25" rot="R90"/>
<smd name="17" x="-0.6" y="-2.945" dx="0.81" dy="0.22" layer="1" roundness="25" rot="R90"/>
<smd name="18" x="-0.2" y="-2.945" dx="0.81" dy="0.22" layer="1" roundness="25" rot="R90"/>
<smd name="19" x="0.2" y="-2.945" dx="0.81" dy="0.22" layer="1" roundness="25" rot="R90"/>
<smd name="20" x="0.6" y="-2.945" dx="0.81" dy="0.22" layer="1" roundness="25" rot="R90"/>
<smd name="21" x="1" y="-2.945" dx="0.81" dy="0.22" layer="1" roundness="25" rot="R90"/>
<smd name="22" x="1.4" y="-2.945" dx="0.81" dy="0.22" layer="1" roundness="25" rot="R90"/>
<smd name="23" x="1.8" y="-2.945" dx="0.81" dy="0.22" layer="1" roundness="25" rot="R90"/>
<smd name="24" x="2.2" y="-2.945" dx="0.81" dy="0.22" layer="1" roundness="25" rot="R90"/>
<smd name="39" x="1.4" y="2.945" dx="0.81" dy="0.22" layer="1" roundness="25" rot="R90"/>
<smd name="40" x="1" y="2.945" dx="0.81" dy="0.22" layer="1" roundness="25" rot="R90"/>
<smd name="41" x="0.6" y="2.945" dx="0.81" dy="0.22" layer="1" roundness="25" rot="R90"/>
<smd name="42" x="0.2" y="2.945" dx="0.81" dy="0.22" layer="1" roundness="25" rot="R90"/>
<smd name="43" x="-0.2" y="2.945" dx="0.81" dy="0.22" layer="1" roundness="25" rot="R90"/>
<smd name="44" x="-0.6" y="2.945" dx="0.81" dy="0.22" layer="1" roundness="25" rot="R90"/>
<smd name="45" x="-1" y="2.945" dx="0.81" dy="0.22" layer="1" roundness="25" rot="R90"/>
<smd name="46" x="-1.4" y="2.945" dx="0.81" dy="0.22" layer="1" roundness="25" rot="R90"/>
<smd name="47" x="-1.8" y="2.945" dx="0.81" dy="0.22" layer="1" roundness="25" rot="R90"/>
<smd name="48" x="-2.2" y="2.945" dx="0.81" dy="0.22" layer="1" roundness="25" rot="R90"/>
<smd name="1" x="-2.945" y="2.2" dx="0.81" dy="0.22" layer="1" roundness="25"/>
<smd name="2" x="-2.945" y="1.8" dx="0.81" dy="0.22" layer="1" roundness="25"/>
<smd name="3" x="-2.945" y="1.4" dx="0.81" dy="0.22" layer="1" roundness="25"/>
<smd name="4" x="-2.945" y="1" dx="0.81" dy="0.22" layer="1" roundness="25"/>
<smd name="5" x="-2.945" y="0.6" dx="0.81" dy="0.22" layer="1" roundness="25"/>
<smd name="6" x="-2.945" y="0.2" dx="0.81" dy="0.22" layer="1" roundness="25"/>
<smd name="7" x="-2.945" y="-0.2" dx="0.81" dy="0.22" layer="1" roundness="25"/>
<smd name="8" x="-2.945" y="-0.6" dx="0.81" dy="0.22" layer="1" roundness="25"/>
<smd name="9" x="-2.945" y="-1" dx="0.81" dy="0.22" layer="1" roundness="25"/>
<smd name="10" x="-2.945" y="-1.4" dx="0.81" dy="0.22" layer="1" roundness="25"/>
<smd name="11" x="-2.945" y="-1.8" dx="0.81" dy="0.22" layer="1" roundness="25"/>
<smd name="12" x="-2.945" y="-2.2" dx="0.81" dy="0.22" layer="1" roundness="25"/>
<smd name="13" x="-2.2" y="-2.945" dx="0.81" dy="0.22" layer="1" roundness="25" rot="R90"/>
<smd name="14" x="-1.8" y="-2.945" dx="0.81" dy="0.22" layer="1" roundness="25" rot="R90"/>
<smd name="25" x="2.945" y="-2.2" dx="0.81" dy="0.22" layer="1" roundness="25"/>
<smd name="26" x="2.945" y="-1.8" dx="0.81" dy="0.22" layer="1" roundness="25"/>
<smd name="27" x="2.945" y="-1.4" dx="0.81" dy="0.22" layer="1" roundness="25"/>
<smd name="28" x="2.945" y="-1" dx="0.81" dy="0.22" layer="1" roundness="25"/>
<smd name="29" x="2.945" y="-0.6" dx="0.81" dy="0.22" layer="1" roundness="25"/>
<smd name="30" x="2.945" y="-0.2" dx="0.81" dy="0.22" layer="1" roundness="25"/>
<smd name="31" x="2.945" y="0.2" dx="0.81" dy="0.22" layer="1" roundness="25"/>
<smd name="32" x="2.945" y="0.6" dx="0.81" dy="0.22" layer="1" roundness="25"/>
<smd name="33" x="2.945" y="1" dx="0.81" dy="0.22" layer="1" roundness="25"/>
<smd name="34" x="2.945" y="1.4" dx="0.81" dy="0.22" layer="1" roundness="25"/>
<smd name="35" x="2.945" y="1.8" dx="0.81" dy="0.22" layer="1" roundness="25"/>
<smd name="36" x="2.945" y="2.2" dx="0.81" dy="0.22" layer="1" roundness="25"/>
<smd name="37" x="2.2" y="2.945" dx="0.81" dy="0.22" layer="1" roundness="25" rot="R90"/>
<smd name="38" x="1.8" y="2.945" dx="0.81" dy="0.22" layer="1" roundness="25" rot="R90"/>
<smd name="49" x="0" y="0" dx="4.3" dy="4.3" layer="1" cream="no"/>
</package>
</packages>
<symbols>
<symbol name="ESP32-D0WDQ6">
<wire x1="-17.78" y1="43.18" x2="17.78" y2="43.18" width="0.41" layer="94"/>
<wire x1="17.78" y1="43.18" x2="17.78" y2="-43.18" width="0.41" layer="94"/>
<wire x1="17.78" y1="-43.18" x2="-17.78" y2="-43.18" width="0.41" layer="94"/>
<wire x1="-17.78" y1="-43.18" x2="-17.78" y2="43.18" width="0.41" layer="94"/>
<text x="-17.8377" y="44.3233" size="2.089559375" layer="95" ratio="10" rot="SR0">&gt;NAME</text>
<text x="-17.7995" y="-47.2317" size="2.08508125" layer="96" ratio="10" rot="SR0">&gt;VALUE</text>
<pin name="CAP1" x="-22.86" y="25.4" length="middle" direction="in"/>
<pin name="CAP2" x="-22.86" y="22.86" length="middle" direction="in"/>
<pin name="CHIP_PU" x="-22.86" y="17.78" length="middle" direction="in"/>
<pin name="SENSOR_CAPN" x="-22.86" y="15.24" length="middle" direction="in"/>
<pin name="SENSOR_CAPP" x="-22.86" y="12.7" length="middle" direction="in"/>
<pin name="SENSOR_VN" x="-22.86" y="10.16" length="middle" direction="in"/>
<pin name="SENSOR_VP" x="-22.86" y="7.62" length="middle" direction="in"/>
<pin name="VDET_1" x="-22.86" y="5.08" length="middle" direction="in"/>
<pin name="VDET_2" x="-22.86" y="2.54" length="middle" direction="in"/>
<pin name="XTAL_P" x="22.86" y="20.32" length="middle" direction="in" rot="R180"/>
<pin name="32K_XN" x="22.86" y="-25.4" length="middle" rot="R180"/>
<pin name="32K_XP" x="22.86" y="-22.86" length="middle" rot="R180"/>
<pin name="GPIO0" x="-22.86" y="-2.54" length="middle"/>
<pin name="GPIO16" x="-22.86" y="-12.7" length="middle"/>
<pin name="GPIO17" x="-22.86" y="-15.24" length="middle"/>
<pin name="GPIO18" x="-22.86" y="-17.78" length="middle"/>
<pin name="GPIO19" x="-22.86" y="-20.32" length="middle"/>
<pin name="GPIO2" x="-22.86" y="-5.08" length="middle"/>
<pin name="GPIO21" x="-22.86" y="-22.86" length="middle"/>
<pin name="GPIO22" x="-22.86" y="-25.4" length="middle"/>
<pin name="GPIO23" x="-22.86" y="-27.94" length="middle"/>
<pin name="GPIO25" x="-22.86" y="-30.48" length="middle"/>
<pin name="GPIO26" x="-22.86" y="-33.02" length="middle"/>
<pin name="GPIO27" x="-22.86" y="-35.56" length="middle"/>
<pin name="GND" x="22.86" y="-40.64" length="middle" direction="pwr" rot="R180"/>
<pin name="VDD3P3" x="22.86" y="38.1" length="middle" direction="pwr" rot="R180"/>
<pin name="VDD3P3_CPU" x="22.86" y="35.56" length="middle" direction="pwr" rot="R180"/>
<pin name="VDD3P3_RTC" x="22.86" y="33.02" length="middle" direction="pwr" rot="R180"/>
<pin name="VDDA" x="22.86" y="40.64" length="middle" direction="pwr" rot="R180"/>
<pin name="VDD_SDIO" x="22.86" y="30.48" length="middle" direction="pwr" rot="R180"/>
<pin name="XTAL_N" x="22.86" y="17.78" length="middle" direction="out" rot="R180"/>
<pin name="GPIO4" x="-22.86" y="-7.62" length="middle"/>
<pin name="GPIO5" x="-22.86" y="-10.16" length="middle"/>
<pin name="LNA_IN" x="22.86" y="12.7" length="middle" rot="R180"/>
<pin name="MTCK" x="22.86" y="7.62" length="middle" rot="R180"/>
<pin name="MTDI" x="22.86" y="5.08" length="middle" rot="R180"/>
<pin name="MTDO" x="22.86" y="2.54" length="middle" rot="R180"/>
<pin name="MTMS" x="22.86" y="0" length="middle" rot="R180"/>
<pin name="SD_CLK" x="22.86" y="-5.08" length="middle" function="clk" rot="R180"/>
<pin name="SD_CMD" x="22.86" y="-7.62" length="middle" rot="R180"/>
<pin name="SD_DATA_0" x="22.86" y="-10.16" length="middle" rot="R180"/>
<pin name="SD_DATA_1" x="22.86" y="-12.7" length="middle" rot="R180"/>
<pin name="SD_DATA_2" x="22.86" y="-15.24" length="middle" rot="R180"/>
<pin name="SD_DATA_3" x="22.86" y="-17.78" length="middle" rot="R180"/>
<pin name="U0RXD" x="22.86" y="-30.48" length="middle" rot="R180"/>
<pin name="U0TXD" x="22.86" y="-33.02" length="middle" rot="R180"/>
</symbol>
</symbols>
<devicesets>
<deviceset name="ESP32-D0WDQ6" prefix="U">
<description>SoC; 2, 412G÷2, 484GHz; -98dBm; 2.2÷3.6VDC; SMD; QFN48; 19.5dBm; 16MB </description>
<gates>
<gate name="G$1" symbol="ESP32-D0WDQ6" x="0" y="0"/>
</gates>
<devices>
<device name="" package="QFN35P500X500X90-49N">
<connects>
<connect gate="G$1" pin="32K_XN" pad="13"/>
<connect gate="G$1" pin="32K_XP" pad="12"/>
<connect gate="G$1" pin="CAP1" pad="48"/>
<connect gate="G$1" pin="CAP2" pad="47"/>
<connect gate="G$1" pin="CHIP_PU" pad="9"/>
<connect gate="G$1" pin="GND" pad="49"/>
<connect gate="G$1" pin="GPIO0" pad="23"/>
<connect gate="G$1" pin="GPIO16" pad="25"/>
<connect gate="G$1" pin="GPIO17" pad="27"/>
<connect gate="G$1" pin="GPIO18" pad="35"/>
<connect gate="G$1" pin="GPIO19" pad="38"/>
<connect gate="G$1" pin="GPIO2" pad="22"/>
<connect gate="G$1" pin="GPIO21" pad="42"/>
<connect gate="G$1" pin="GPIO22" pad="39"/>
<connect gate="G$1" pin="GPIO23" pad="36"/>
<connect gate="G$1" pin="GPIO25" pad="14"/>
<connect gate="G$1" pin="GPIO26" pad="15"/>
<connect gate="G$1" pin="GPIO27" pad="16"/>
<connect gate="G$1" pin="GPIO4" pad="24"/>
<connect gate="G$1" pin="GPIO5" pad="34"/>
<connect gate="G$1" pin="LNA_IN" pad="2"/>
<connect gate="G$1" pin="MTCK" pad="20"/>
<connect gate="G$1" pin="MTDI" pad="18"/>
<connect gate="G$1" pin="MTDO" pad="21"/>
<connect gate="G$1" pin="MTMS" pad="17"/>
<connect gate="G$1" pin="SD_CLK" pad="31"/>
<connect gate="G$1" pin="SD_CMD" pad="30"/>
<connect gate="G$1" pin="SD_DATA_0" pad="32"/>
<connect gate="G$1" pin="SD_DATA_1" pad="33"/>
<connect gate="G$1" pin="SD_DATA_2" pad="28"/>
<connect gate="G$1" pin="SD_DATA_3" pad="29"/>
<connect gate="G$1" pin="SENSOR_CAPN" pad="7"/>
<connect gate="G$1" pin="SENSOR_CAPP" pad="6"/>
<connect gate="G$1" pin="SENSOR_VN" pad="8"/>
<connect gate="G$1" pin="SENSOR_VP" pad="5"/>
<connect gate="G$1" pin="U0RXD" pad="40"/>
<connect gate="G$1" pin="U0TXD" pad="41"/>
<connect gate="G$1" pin="VDD3P3" pad="3 4"/>
<connect gate="G$1" pin="VDD3P3_CPU" pad="37"/>
<connect gate="G$1" pin="VDD3P3_RTC" pad="19"/>
<connect gate="G$1" pin="VDDA" pad="1 43 46"/>
<connect gate="G$1" pin="VDD_SDIO" pad="26"/>
<connect gate="G$1" pin="VDET_1" pad="10"/>
<connect gate="G$1" pin="VDET_2" pad="11"/>
<connect gate="G$1" pin="XTAL_N" pad="44"/>
<connect gate="G$1" pin="XTAL_P" pad="45"/>
</connects>
<technologies>
<technology name="">
<attribute name="AVAILABILITY" value="Unavailable"/>
<attribute name="DESCRIPTION" value=" IC RF TxRx + MCU Bluetooth, WiFi 802.11b/g/n, Bluetooth 4.2 2.412GHz ~ 2.484GHz 48-VFQFN Exposed Pad "/>
<attribute name="MF" value="Espressif Systems"/>
<attribute name="MP" value="ESP32-D0WDQ6"/>
<attribute name="PACKAGE" value="VFQFN-48 Espressif Systems"/>
<attribute name="PRICE" value="None"/>
</technology>
</technologies>
</device>
</devices>
</deviceset>
</devicesets>
</library>
<library name="FPC241H-040G10M">
<packages>
</packages>
<symbols>
</symbols>
<devicesets>
<deviceset name="FPC241H-040G10M">
<gates>
</gates>
<devices>
<device name="">
<technologies>
<technology name=""/>
</technologies>
</device>
</devices>
</deviceset>
</devicesets>
</library>
</libraries>
<attributes>
</attributes>
<variantdefs>
</variantdefs>
<classes>
<class number="0" name="default" width="0" drill="0">
</class>
</classes>
<parts>
<part name="U1" library="ESP32-D0WDQ6" deviceset="ESP32-D0WDQ6" device=""/>
<part name="U$1" library="FPC241H-040G10M" deviceset="FPC241H-040G10M" device=""/>
</parts>
<sheets>
<sheet>
<plain>
</plain>
<instances>
<instance part="U1" gate="G$1" x="40.64" y="55.88" smashed="yes">
<attribute name="NAME" x="22.8023" y="100.2033" size="2.089559375" layer="95" ratio="10" rot="SR0"/>
<attribute name="VALUE" x="22.8405" y="8.6483" size="2.08508125" layer="96" ratio="10" rot="SR0"/>
</instance>
</instances>
<busses>
</busses>
<nets>
</nets>
</sheet>
</sheets>
</schematic>
</drawing>
</eagle>
