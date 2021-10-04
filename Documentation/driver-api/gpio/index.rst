===================================
General Purpose Input/Output (GPIO)
===================================

Contents:

.. toctree::
   :maxdepth: 2

   intro
   driver
   consumer
   board
   drivers-on-gpio
   legacy

Core
====

.. kernel-doc:: include/linux/gpio/driver.h
   :internal:

.. kernel-doc:: drivers/gpio/gpiolib.c
   :export:

ACPI support
============

.. kernel-doc:: drivers/gpio/gpiolib-acpi.c
   :export:

Device tree support
===================

.. kernel-doc:: drivers/gpio/gpiolib-of.c
   :export:

Device-managed API
==================

.. kernel-doc:: drivers/gpio/devres.c
   :export:

sysfs helpers
=============

.. kernel-doc:: drivers/gpio/gpiolib-sysfs.c
   :export:



文档	                简介
index.rst	         文档目录和源码清单
intro.rst	         gpio 简介
driver.rst	         描述如何编写 gpio controller driver
consumer.rst	      描述 gpio consumer 如何使用 gpio
board.rst	         描述设备如何申请 gpio
drivers-on-gpio.rst	列举一些使用了gpio子系统的常见驱动，例如 leds-gpio.c、gpio_keys.c 等
legacy.rst	         描述 legacy gpio 接口