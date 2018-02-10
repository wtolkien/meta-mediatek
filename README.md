# meta-mediatek

An OpenEmbedded BSP layer for Mediatek's MT7623 SoC

## Overview

Mediatek's MT7623 is a Quad-core ARM® Cortex-A7 SoC running at 1.3GHz. It contains  many peripherals such as USB3.0, PCIe, Network  processor, Crypto engine, etc.

![MT7623](https://github.com/wtolkien/meta-mediatek/blob/master/MT7623.jpg)

There are two variants of this chip:
* MT7623A: has a built-in 5-port Gigabit switch (MT7530), but no graphics
* MT7623N: has a built-in graphic engine, but no Ethernet switch

Mediatek ships an outdated 3.10 kernel with their BSP, however the chip is now reasonably well supported by upstream Linux starting with kernel 4.14. 

Currently, this BSP layer uses Linux 4.9 with patches from the Lede project, a switch to kernel 4.14 is planned. 

As of 02/2018 there are at least four hardware platforms available that use the MT7623:

* the original MTK evaluation board (MT7623A), which is offered by [AsiaRF](http://www.asiarf.com/product-view-404.html) as the 'GeekForce' board.
* The Banana Pi R2 (MT7623N) from [SinoVoip](http://www.banana-pi.org/r2.html), probably the most widely available board.
* A SoM (MT7623A) from [Gainstrong](https://gainstrong.en.alibaba.com).
* A relatively new router platform from [UniElec](http://www.unielecinc.com/q/news/cn/p/product/detail.html?qd_guid=OjXwKCaRlN).

## Current Status and Limitations

* This layer is compatible with Yocto release 'rocko'. Other Yocto release may or may not work.
* Development has been done on the MTK eval board (MACHINE: mt7623-evb), using eMMC memory. Only this board has been tested and is currently supported
* This layer supports a target image 'core-image-mtk' which is based on 'core-image-minimal' with the addition of an SSH server and support for [swupdate](https://sbabic.github.io/swupdate/swupdate.html). It depends on the [meta-swupdate](https://github.com/sbabic/meta-swupdate) layer
* The build process creates a complete suite of files suitable for uploading into eMMC using MTK's [flashtool](https://spflashtool.com/)
* The resulting image launches a minimal web server at port 8080 to allow for subsequent software updates using .swu images (which are also generated)
* The eMMC is partitioned into two identical rootfs partitions (for redundancy, as supported by swupdate) and an additional persistent data partition. 

## Installation

The following steps have been tested on Ubuntu 16.04, but will most likely work on other Linux distributions as well:

Setup Yocto Rocko:
```
git clone -b rocko git://git.yoctoproject.org/poky.git yocto-rocko
```

Add other required layers including this one:
```
cd yocto-rocko
git clone -b rocko git://git.openembedded.org/meta-openembedded
git clone -b rocko https://github.com/sbabic/meta-swupdate
git clone https://github.com/wtolkien/meta-mediatek/
```

Initialize your Yocto installation to generate basic config files
```
source oe-init-build-env
```

Edit the file `yocto-rocko/build/conf/bblayers.conf` to add those three additional bitbake layers to your `BBLAYERS` variable so that it looks like this:
```
BBLAYERS ?= " \
  /[your existing path]/yocto-rocko/meta \
  /[your existing path]/yocto-rocko/meta-poky \
  /[your existing path]/yocto-rocko/meta-yocto-bsp \
  ${TOPDIR}/../meta-mediatek \
  ${TOPDIR}/../meta-swupdate \
  ${TOPDIR}/../meta-openembedded/meta-oe \
  "
```
(Use common sense if your paths vary or if your file looks slightly different.)

That's it! You can now build firmware images for the MT7623 SoC.

## Build Instructions

Initialize Yocto and specify your target board. Currently only the Mediatek eval board (AsiaRF 'Geekforce' board) is supported. This board is called `mt7623-evb`.
```
cd yocto-rocko
source oe-init-build-env
export MACHINE=mt7623-evb
```

Start the build. Depending on your computer and internet connection, the initial build can take several hours! Subsequent builds will be much faster.
``` 
[cd yocto-rocko/build]
bitbake core-image-mtk-swu
```

When completed, the resulting image files will be in the following directory:
```
yocto-rocko/build/tmp/deploy/images/mt7623-evb
```

## Installation

The build process generates two different images:

1.) A set of files that can be used with Mediatek's [flashtool](https://spflashtool.com/). Flashtool requires a 'scatter file' which specifies all other components to be loaded into eMMC. The Yocto build process will generate these files automatically. After installing 'flashtool', click on the 'download' tab:

![Flashtool](https://github.com/wtolkien/meta-mediatek/blob/master/sp-flash-tool.jpg)

 * select the `MTK_AllInOne_DA.bin` Download agent that is part of the flashtool archive
 * select `scatter-mt7623-evb.txt` from the Yocto deploy directoy above as your 'Scatter-loading' file. Flashtool will pick up all other required files from the deploy directory. These are:
 ```
 preloader-mt7623-evb.bin
 gpt-mt7623-evb.bin
 u-boot-mt7623-evb.bin
 u-boot-env-mt7623-evb.bin
 core-image-mtk-mt7623-evb.ext4
 ```
 * click on the 'Download' button
 * make sure the MT7623 board is connected to the computer running 'flashtool' via USB, then power-cycle the MT7623 board. When it powers on again, 'flashtool' should detect the board and start programming the eMMC.
 * power-cycle the MT7623 board when the process is complete. The board should now boot into Linux. Default login is 'root' with no password.
 
 2.) The second image can be used with the 'swupdate' tool that's part of the image. 'swupdate' provides a web interface at port 8080. Select the file `core-image-mtk-swu-mt7623-evb.swu` to upgrade the firmware. Note that this will only re-flash the root file system (including Linux kernel, which is in the /boot directory), but not the preloader, partition table or u-boot. 
 
 





