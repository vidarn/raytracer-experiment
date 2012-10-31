#!/bin/sh

cp out.tif out_old.tif
./raytracer
qiv out.tif out_old.tif
