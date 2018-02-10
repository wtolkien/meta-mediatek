# meta-mediatek

An OpenEmbedded BSP layer to support Mediatek's MT7623 SoC

## Overview

Mediatek's MT7623 is a Quad-core ARM® Cortex-A7 SoC running at 1.3GHz. It contains a many peripherals such as USB3.0, PCIe, Network  processor, Crypto engine, etc.

There are two variants of this chip:
* MT7623A: has a built-in 5-port Gigabit switch (MT7530), but no graphics
* MT7623N: has a built-in graphic engine, but no Ethernet switch

Mediatek ships an outdated 3.10 kernel with their BSP, however the chip is now reasonably well supported by upstream Linux starting with kernel 4.14. 

Currently, this BSP layer uses Linux 4.9 with patches from the Lede project, a switch to kernel 4.14 is planned. 

As of 02/2018 there are at least four hardware platforms available that use the MT7623:

* the original MTK evaluation board (MT7623A), which is offered by [AsiaRF](http://www.asiarf.com/product-view-404.html) as the 'GeekForce' board
* The Banana Pi R2 (MT7623N) from [SinoVoip](http://www.banana-pi.org/r2.html), probably the most widely available board
* A SoM (MT7623A) from [Gainstrong](https://gainstrong.en.alibaba.com)
* A relatively new router platform from [UniElec](http://www.unielecinc.com/q/news/cn/p/product/detail.html?qd_guid=OjXwKCaRlN)

## Status and Limitations

* This layer is compatible with Yocto release 'Rocko'. Other Yocto release have not been tested.
* Development has been done on the MTK eval board (MACHINE: mt7623-evb), using eMMC memory. Only this board has been tested and is currently supported
* This layer supports a target image 'core-image-mtk' which is based on 'core-image-minimal' with the addition of an SSH server and support for [swupdate](https://sbabic.github.io/swupdate/swupdate.html). It depends on the [meta-swupdate](https://github.com/sbabic/meta-swupdate) layer
* The build process creates a complete suite of files suitable for uploading into eMMC using MTK's [flashtool](https://spflashtool.com/)
* The resulting image launches a minimal web server at port 8080 to allow for subsequent software updates using .swu images (which are also generated)
* The eMMC is partitioned into two identical rootfs partitions (for redundancy, as supported by swupdate) and an additional persistent data partition. 

## Installation
---------------

Setup Yocto Rocko:
```
git clone -b rocko git://git.yoctoproject.org/poky.git yocto-rocko
```

Add other required layers including this one:
```
cd yocto-rocko
git clone -b git://git.openembedded.org/meta-openembedded
git clone -b https://github.com/sbabic/meta-swupdate
git clone https://github.com/wtolkien/meta-mediatek/
```

Initialize your Yocto installtion to generate basic config files
```
source oe-init-build-env
```

Edit the file `yocto-rocko/build/conf/` to add those three additional bitbake layers to your `BBLAYERS` variable. Use common sense of your paths vary or if your file looks slightly different.
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
That's it! You can now build firmware images for the MT7623 SoC.

## Build Instructions
---------------------





