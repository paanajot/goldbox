# Any commands which fail will cause the shell script to exit immediately
set -e

echo "SCRIPT GENERATE_TEST_FILES - START"

export TEST_NAME=testText10
export OUTDIR=./generated_files/$TEST_NAME
mkdir -p ${OUTDIR}
./testfilegenerator --file=$OUTDIR/$TEST_NAME.bin --size=10 --fill=text
./bin2cpp --noheader --file=$OUTDIR/$TEST_NAME.bin --output=$OUTDIR --headerfile=_$TEST_NAME.h --identifier=$TEST_NAME  --chunksize=450 --override

export TEST_NAME=testText1000
export OUTDIR=./generated_files/$TEST_NAME
mkdir -p ${OUTDIR}
./testfilegenerator --file=$OUTDIR/$TEST_NAME.bin --size=1000 --fill=text
./bin2cpp --noheader --file=$OUTDIR/$TEST_NAME.bin --output=$OUTDIR --headerfile=_$TEST_NAME.h --identifier=$TEST_NAME  --chunksize=450 --override

export TEST_NAME=testText100000
export OUTDIR=./generated_files/$TEST_NAME
mkdir -p ${OUTDIR}
./testfilegenerator --file=$OUTDIR/$TEST_NAME.bin --size=100000 --fill=text
./bin2cpp --noheader --file=$OUTDIR/$TEST_NAME.bin --output=$OUTDIR --headerfile=_$TEST_NAME.h --identifier=$TEST_NAME  --chunksize=450 --override

export TEST_NAME=testRandom1
export OUTDIR=./generated_files/$TEST_NAME
mkdir -p ${OUTDIR}
./testfilegenerator --file=$OUTDIR/$TEST_NAME.bin --size=100000 --fill=random --seed=1
./bin2cpp --noheader --file=$OUTDIR/$TEST_NAME.bin --output=$OUTDIR --headerfile=_$TEST_NAME.h --identifier=$TEST_NAME  --chunksize=450 --override

export TEST_NAME=testRandom2
export OUTDIR=./generated_files/$TEST_NAME
mkdir -p ${OUTDIR}
./testfilegenerator --file=$OUTDIR/$TEST_NAME.bin --size=100000 --fill=random --seed=2
./bin2cpp --noheader --file=$OUTDIR/$TEST_NAME.bin --output=$OUTDIR --headerfile=_$TEST_NAME.h --identifier=$TEST_NAME  --chunksize=450 --override

export TEST_NAME=testRandom3
export OUTDIR=./generated_files/$TEST_NAME
mkdir -p ${OUTDIR}
./testfilegenerator --file=$OUTDIR/$TEST_NAME.bin --size=100000 --fill=random --seed=3
./bin2cpp --noheader --file=$OUTDIR/$TEST_NAME.bin --output=$OUTDIR --headerfile=_$TEST_NAME.h --identifier=$TEST_NAME  --chunksize=450 --override

export TEST_NAME=testRandom4
export OUTDIR=./generated_files/$TEST_NAME
mkdir -p ${OUTDIR}
./testfilegenerator --file=$OUTDIR/$TEST_NAME.bin --size=1000 --fill=random --seed=4
./bin2cpp --noheader --file=$OUTDIR/$TEST_NAME.bin --output=$OUTDIR --headerfile=_$TEST_NAME.h --identifier=$TEST_NAME --chunksize=200 --override

export TEST_NAME=testRandom5
export OUTDIR=./generated_files/$TEST_NAME
mkdir -p ${OUTDIR}
./testfilegenerator --file=$OUTDIR/$TEST_NAME.bin --size=1000 --fill=random --seed=5
./bin2cpp --noheader --file=$OUTDIR/$TEST_NAME.bin --output=$OUTDIR --headerfile=_$TEST_NAME.h --identifier=$TEST_NAME --chunksize=200 --override

export TEST_NAME=testRandom6
export OUTDIR=./generated_files/$TEST_NAME
mkdir -p ${OUTDIR}
./testfilegenerator --file=$OUTDIR/$TEST_NAME.bin --size=1000 --fill=random --seed=6
./bin2cpp --noheader --file=$OUTDIR/$TEST_NAME.bin --output=$OUTDIR --headerfile=_$TEST_NAME.h --identifier=$TEST_NAME --chunksize=200 --override

export TEST_NAME=testRandom7
export OUTDIR=./generated_files/$TEST_NAME
mkdir -p ${OUTDIR}
./testfilegenerator --file=$OUTDIR/$TEST_NAME.bin --size=1000 --fill=random --seed=7
./bin2cpp --noheader --file=$OUTDIR/$TEST_NAME.bin --output=$OUTDIR --headerfile=_$TEST_NAME.h --identifier=$TEST_NAME --chunksize=200 --override

export TEST_NAME=testRandom8
export OUTDIR=./generated_files/$TEST_NAME
mkdir -p ${OUTDIR}
./testfilegenerator --file=$OUTDIR/$TEST_NAME.bin --size=1000 --fill=random --seed=8
./bin2cpp --noheader --file=$OUTDIR/$TEST_NAME.bin --output=$OUTDIR --headerfile=_$TEST_NAME.h --identifier=$TEST_NAME --chunksize=200 --override

export TEST_NAME=testHtml100000
export OUTDIR=./generated_files/$TEST_NAME
mkdir -p ${OUTDIR}
./testfilegenerator --file=$OUTDIR/$TEST_NAME.bin --size=100000 --fill=html
./bin2cpp --noheader --file=$OUTDIR/$TEST_NAME.bin --output=$OUTDIR --headerfile=_$TEST_NAME.h --identifier=$TEST_NAME  --chunksize=450 --override

export TEST_NAME=testSequential1000
export OUTDIR=./generated_files/$TEST_NAME
mkdir -p ${OUTDIR}
./testfilegenerator --file=$OUTDIR/$TEST_NAME.bin --size=1000 --fill=sequential
./bin2cpp --noheader --file=$OUTDIR/$TEST_NAME.bin --output=$OUTDIR --headerfile=_$TEST_NAME.h --identifier=$TEST_NAME  --chunksize=180 --override

export TEST_NAME=testSequential10MB
export OUTDIR=./generated_files/$TEST_NAME
mkdir -p ${OUTDIR}
./testfilegenerator --file=$OUTDIR/$TEST_NAME.bin --size=10485760 --fill=sequential
./bin2cpp --noheader --file=$OUTDIR/$TEST_NAME.bin --output=$OUTDIR --headerfile=_$TEST_NAME.h --identifier=$TEST_NAME  --chunksize=450 --override

#   export TEST_NAME=testSequential20MB
#   export OUTDIR=./generated_files/$TEST_NAME
#   mkdir -p ${OUTDIR}
#   ./testfilegenerator --file=$OUTDIR/$TEST_NAME.bin --size=20971520 --fill=sequential
#   ./bin2cpp --noheader --file=$OUTDIR/$TEST_NAME.bin --output=$OUTDIR --headerfile=_$TEST_NAME.h --identifier=$TEST_NAME  --chunksize=450 --override
#   
#   export TEST_NAME=testSequential30MB
#   export OUTDIR=./generated_files/$TEST_NAME
#   mkdir -p ${OUTDIR}
#   ./testfilegenerator --file=$OUTDIR/$TEST_NAME.bin --size=31457280 --fill=sequential
#   ./bin2cpp --noheader --file=$OUTDIR/$TEST_NAME.bin --output=$OUTDIR --headerfile=_$TEST_NAME.h --identifier=$TEST_NAME  --chunksize=450 --override

export TEST_NAME=testGeneratorArray10000
export OUTDIR=./generated_files/$TEST_NAME
mkdir -p ${OUTDIR}
./testfilegenerator --file=$OUTDIR/$TEST_NAME.bin --size=10000 --fill=sequential
./bin2cpp --noheader --file=$OUTDIR/$TEST_NAME.bin --output=$OUTDIR --headerfile=_$TEST_NAME.h --identifier=$TEST_NAME  --chunksize=450 --generator=array --override

export TEST_NAME=testGeneratorString10000
export OUTDIR=./generated_files/$TEST_NAME
mkdir -p ${OUTDIR}
./testfilegenerator --file=$OUTDIR/$TEST_NAME.bin --size=10000 --fill=sequential
./bin2cpp --noheader --file=$OUTDIR/$TEST_NAME.bin --output=$OUTDIR --headerfile=_$TEST_NAME.h --identifier=$TEST_NAME  --chunksize=450 --generator=string --override

export TEST_NAME=testGeneratorSegment10000
export OUTDIR=./generated_files/$TEST_NAME
mkdir -p ${OUTDIR}
./testfilegenerator --file=$OUTDIR/$TEST_NAME.bin --size=10000 --fill=sequential
./bin2cpp --noheader --file=$OUTDIR/$TEST_NAME.bin --output=$OUTDIR --headerfile=_$TEST_NAME.h --identifier=$TEST_NAME  --chunksize=450 --generator=segment --override

export TEST_NAME=testGeneratorWin32
export OUTDIR=./generated_files/$TEST_NAME
mkdir -p ${OUTDIR}
./testfilegenerator --file=$OUTDIR/$TEST_NAME.bin --size=10000 --fill=sequential
./bin2cpp --noheader --file=$OUTDIR/$TEST_NAME.bin --output=$OUTDIR --headerfile=_$TEST_NAME.h --identifier=$TEST_NAME  --chunksize=450 --generator=win32 --override

export TEST_NAME=testNamespace
export OUTDIR=./generated_files/$TEST_NAME
mkdir -p ${OUTDIR}
./testfilegenerator --file=$OUTDIR/$TEST_NAME.bin --size=10000 --fill=sequential
./bin2cpp --noheader --file=$OUTDIR/$TEST_NAME.bin --output=$OUTDIR --headerfile=_$TEST_NAME.h --identifier=$TEST_NAME  --chunksize=450 --generator=segment --override --namespace=foobar

export TEST_NAME=testBaseClass
export OUTDIR=./generated_files/$TEST_NAME
mkdir -p ${OUTDIR}
./testfilegenerator --file=$OUTDIR/$TEST_NAME.bin --size=10000 --fill=sequential
./bin2cpp --noheader --file=$OUTDIR/$TEST_NAME.bin --output=$OUTDIR --headerfile=_$TEST_NAME.h --identifier=$TEST_NAME  --chunksize=450 --generator=segment --override --baseclass=Resource

export TEST_NAME=testEncodingOct
export OUTDIR=./generated_files/$TEST_NAME
mkdir -p ${OUTDIR}
./testfilegenerator --file=$OUTDIR/$TEST_NAME.bin --size=300 --fill=sequential
./bin2cpp --noheader --file=$OUTDIR/$TEST_NAME.bin --output=$OUTDIR --headerfile=_$TEST_NAME.h --identifier=$TEST_NAME  --chunksize=40 --generator=segment --override --encoding=OcT

export TEST_NAME=testEncodingHex
export OUTDIR=./generated_files/$TEST_NAME
mkdir -p ${OUTDIR}
./testfilegenerator --file=$OUTDIR/$TEST_NAME.bin --size=300 --fill=sequential
./bin2cpp --noheader --file=$OUTDIR/$TEST_NAME.bin --output=$OUTDIR --headerfile=_$TEST_NAME.h --identifier=$TEST_NAME  --chunksize=40 --generator=segment --override --encoding=HeX

export TEST_NAME=testIssue12
export OUTDIR=./generated_files/$TEST_NAME
mkdir -p ${OUTDIR}
./testfilegenerator --file=$OUTDIR/$TEST_NAME.bin --size=100000 --fill=random --seed=3
./bin2cpp --noheader --file=$OUTDIR/$TEST_NAME.bin --output=$OUTDIR --headerfile=_$TEST_NAME.h --identifier=$TEST_NAME  --chunksize=450 --override

export TEST_NAME=testIssue12Mini
export OUTDIR=./generated_files/$TEST_NAME
mkdir -p ${OUTDIR}
./testfilegenerator --file=$OUTDIR/$TEST_NAME.bin --size=100 --fill=random --seed=3 --skip=47240
./bin2cpp --noheader --file=$OUTDIR/$TEST_NAME.bin --output=$OUTDIR --headerfile=_$TEST_NAME.h --identifier=$TEST_NAME --chunksize=10 --override

export TEST_NAME=testIssue13
export OUTDIR=./generated_files/$TEST_NAME
mkdir -p ${OUTDIR}
./testfilegenerator --file=$OUTDIR/$TEST_NAME.bin --size=1000 --fill=random --seed=1
./bin2cpp --noheader --file=$OUTDIR/$TEST_NAME.bin --output=$OUTDIR --headerfile=_$TEST_NAME.h --identifier=$TEST_NAME --chunksize=100 --override

# Issue #28
export TEST_NAME=testFileManager
export OUTDIR=./generated_files/$TEST_NAME
mkdir -p ${OUTDIR}
./testfilegenerator --file=$OUTDIR/$TEST_NAME.1.bin --size=1000 --fill=random --seed=1
./testfilegenerator --file=$OUTDIR/$TEST_NAME.2.bin --size=1000 --fill=random --seed=2
./bin2cpp --noheader --file=$OUTDIR/$TEST_NAME.1.bin --output=$OUTDIR --headerfile=_$TEST_NAME.1.h --identifier=testFileManager1 --managerfile=FileManager.h --override
./bin2cpp --noheader --file=$OUTDIR/$TEST_NAME.2.bin --output=$OUTDIR --headerfile=_$TEST_NAME.2.h --identifier=testFileManager2 --registerfile --override

export TEST_NAME=testDir01
export OUTDIR=./generated_files/$TEST_NAME
mkdir -p ${OUTDIR}
mkdir -p ${OUTDIR}/images
mkdir -p ${OUTDIR}/sources
./testfilegenerator --file=$OUTDIR/images/IMG_0001.jpg --size=1010 --fill=random --seed=1
./testfilegenerator --file=$OUTDIR/images/IMG_0002.jpg --size=1020 --fill=random --seed=2
./testfilegenerator --file=$OUTDIR/images/IMG_0003.jpg --size=1030 --fill=random --seed=3
./testfilegenerator --file=$OUTDIR/images/IMG_0004.jpg --size=1040 --fill=random --seed=4
./testfilegenerator --file=$OUTDIR/images/IMG_0005.jpg --size=1050 --fill=random --seed=5
./bin2cpp --noheader --file=$OUTDIR/images/IMG_0001.jpg --output=$OUTDIR/sources --headerfile=_img0001.h --identifier=Img0001 --chunksize=200 --override
./bin2cpp --noheader --file=$OUTDIR/images/IMG_0002.jpg --output=$OUTDIR/sources --headerfile=_img0002.h --identifier=Img0002 --chunksize=200 --override
./bin2cpp --noheader --file=$OUTDIR/images/IMG_0003.jpg --output=$OUTDIR/sources --headerfile=_img0003.h --identifier=Img0003 --chunksize=200 --override
./bin2cpp --noheader --file=$OUTDIR/images/IMG_0004.jpg --output=$OUTDIR/sources --headerfile=_img0004.h --identifier=Img0004 --chunksize=200 --override
./bin2cpp --noheader --file=$OUTDIR/images/IMG_0005.jpg --output=$OUTDIR/sources --headerfile=_img0005.h --identifier=Img0005 --chunksize=200 --override

export TEST_NAME=testIssue47
export OUTDIR=./generated_files/$TEST_NAME
mkdir -p ${OUTDIR}
mkdir -p ${OUTDIR}/generated
./testfilegenerator --file=$OUTDIR/$TEST_NAME.bin --size=1000 --fill=random --seed=1
./bin2cpp --noheader --file=$OUTDIR/$TEST_NAME.bin --output=$OUTDIR --headerfile=generated/_$TEST_NAME.h --identifier=$TEST_NAME --managerfile=generated/FileManager47.h --registerfile --namespace=bin2cpp47 --override

export TEST_NAME=testIssue50
export OUTDIR=./generated_files/$TEST_NAME
mkdir -p ${OUTDIR}
./testfilegenerator --file=$OUTDIR/$TEST_NAME.bin --size=1000 --fill=random --seed=1
./bin2cpp --noheader --file=$OUTDIR/$TEST_NAME.bin --output=$OUTDIR --headerfile=_$TEST_NAME.h --identifier=$TEST_NAME --managerfile=FileManager50.h --registerfile --namespace=bin2cpp50 --override

export TEST_NAME=testIssue56a
export OUTDIR=./generated_files/$TEST_NAME
mkdir -p ${OUTDIR}
mkdir -p ${OUTDIR}/input_files
mkdir -p ${OUTDIR}/compiled_sources
mkdir -p ${OUTDIR}/saved_files
./testfilegenerator --file=${OUTDIR}/input_files/_$TEST_NAME.index.1234.css --size=1010 --fill=text
./testfilegenerator --file=${OUTDIR}/input_files/_$TEST_NAME.index.4321.css --size=1020 --fill=text
./bin2cpp --noheader --dir=${OUTDIR}/input_files --output=${OUTDIR}/compiled_sources --chunksize=200 --managerfile=FileManager56a.h --registerfile --namespace=issue56a --override

export TEST_NAME=testIssue56b
export OUTDIR=./generated_files/$TEST_NAME
mkdir -p ${OUTDIR}
mkdir -p ${OUTDIR}/input_files
mkdir -p ${OUTDIR}/input_files/www
mkdir -p ${OUTDIR}/input_files/www/home
mkdir -p ${OUTDIR}/input_files/www/blog
mkdir -p ${OUTDIR}/input_files/www/blog/how-to-create-a-web-site
mkdir -p ${OUTDIR}/input_files/www/blog/using-bin2cpp
mkdir -p ${OUTDIR}/input_files/www/contact
mkdir -p ${OUTDIR}/generated_sources
mkdir -p ${OUTDIR}/saved_files
./testfilegenerator --size=1010 --fill=html --file=${OUTDIR}/input_files/www/index.html
./testfilegenerator --size=1010 --fill=html --file=${OUTDIR}/input_files/www/home/index.html
./testfilegenerator --size=1010 --fill=html --file=${OUTDIR}/input_files/www/blog/index.html
./testfilegenerator --size=1010 --fill=html --file=${OUTDIR}/input_files/www/blog/how-to-create-a-web-site/index.html
./testfilegenerator --size=1010 --fill=html --file=${OUTDIR}/input_files/www/blog/using-bin2cpp/index.html
./testfilegenerator --size=1010 --fill=html --file=${OUTDIR}/input_files/www/contact/index.html
./bin2cpp --noheader --dir=${OUTDIR}/input_files --output=${OUTDIR}/generated_sources --chunksize=200 --managerfile=FileManager56b.h --registerfile --namespace=issue56b --override --keepdirs

export TEST_NAME=testIssue56c
export OUTDIR=./generated_files/$TEST_NAME
mkdir -p ${OUTDIR}
mkdir -p ${OUTDIR}/input_files
mkdir -p ${OUTDIR}/input_files/www
mkdir -p ${OUTDIR}/input_files/www/home
mkdir -p ${OUTDIR}/input_files/www/blog
mkdir -p ${OUTDIR}/input_files/www/blog/how-to-create-a-web-site
mkdir -p ${OUTDIR}/input_files/www/blog/using-bin2cpp
mkdir -p ${OUTDIR}/input_files/www/contact
mkdir -p ${OUTDIR}/generated_sources
mkdir -p ${OUTDIR}/saved_files
./testfilegenerator --size=1010 --fill=html --file=${OUTDIR}/input_files/www/index.html
./testfilegenerator --size=1010 --fill=html --file=${OUTDIR}/input_files/www/home/index.html
./testfilegenerator --size=1010 --fill=html --file=${OUTDIR}/input_files/www/blog/index.html
./testfilegenerator --size=1010 --fill=html --file=${OUTDIR}/input_files/www/blog/how-to-create-a-web-site/index.html
./testfilegenerator --size=1010 --fill=html --file=${OUTDIR}/input_files/www/blog/using-bin2cpp/index.html
./testfilegenerator --size=1010 --fill=html --file=${OUTDIR}/input_files/www/contact/index.html
./bin2cpp --noheader --dir=${OUTDIR}/input_files --output=${OUTDIR}/generated_sources --chunksize=200 --managerfile=FileManager56c.h --registerfile --namespace=issue56c --override

export TEST_NAME=testKeepDirectories
export OUTDIR=./generated_files/$TEST_NAME
mkdir -p ${OUTDIR}
mkdir -p ${OUTDIR}/input_files
mkdir -p ${OUTDIR}/input_files/www
mkdir -p ${OUTDIR}/input_files/www/home
mkdir -p ${OUTDIR}/input_files/www/blog
mkdir -p ${OUTDIR}/input_files/www/blog/how-to-create-a-web-site
mkdir -p ${OUTDIR}/input_files/www/blog/using-bin2cpp
mkdir -p ${OUTDIR}/input_files/www/contact
mkdir -p ${OUTDIR}/generated_sources
mkdir -p ${OUTDIR}/saved_files
./testfilegenerator --size=1010 --fill=html --file=${OUTDIR}/input_files/www/index.html
./testfilegenerator --size=1010 --fill=html --file=${OUTDIR}/input_files/www/home/index.html
./testfilegenerator --size=1010 --fill=html --file=${OUTDIR}/input_files/www/blog/index.html
./testfilegenerator --size=1010 --fill=html --file=${OUTDIR}/input_files/www/blog/how-to-create-a-web-site/index.html
./testfilegenerator --size=1010 --fill=html --file=${OUTDIR}/input_files/www/blog/using-bin2cpp/index.html
./testfilegenerator --size=1010 --fill=html --file=${OUTDIR}/input_files/www/contact/index.html
./bin2cpp --noheader --dir=${OUTDIR}/input_files --output=${OUTDIR}/generated_sources --chunksize=200 --managerfile=FileManagerKeepDirs.h --registerfile --namespace=testkeepdirs --override --keepdirs

export TEST_NAME=testReportedPathFile1
export OUTDIR=./generated_files/$TEST_NAME
mkdir -p $OUTDIR
./testfilegenerator --file=$OUTDIR/$TEST_NAME.bin --size=1024 --fill=random --seed=1
./bin2cpp --noheader --file=$OUTDIR/$TEST_NAME.bin --output=$OUTDIR --chunksize=50 --headerfile=_$TEST_NAME.h --identifier=$TEST_NAME --namespace=testReportedPathFile1 --override --reportedfilepath=foo/bar/$TEST_NAME.bin

# Test option --reportedfilepath where the reported filename does not match the original filename.
export TEST_NAME=testReportedPathFile2
export OUTDIR=./generated_files/$TEST_NAME
mkdir -p $OUTDIR
./testfilegenerator --file=$OUTDIR/$TEST_NAME.bin --size=1024 --fill=random --seed=2
./bin2cpp --noheader --file=$OUTDIR/$TEST_NAME.bin --output=$OUTDIR --chunksize=50 --headerfile=_$TEST_NAME.h --identifier=$TEST_NAME --namespace=testReportedPathFile2 --override --reportedfilepath=ham/eggs/breakfast.bin

export TEST_NAME=testReportedPathDir
export OUTDIR=./generated_files/$TEST_NAME
mkdir -p $OUTDIR
mkdir -p $OUTDIR/input_files
mkdir -p $OUTDIR/input_files/www
mkdir -p $OUTDIR/input_files/www/home
mkdir -p $OUTDIR/input_files/www/blog
mkdir -p $OUTDIR/input_files/www/blog/how-to-create-a-web-site
mkdir -p $OUTDIR/input_files/www/blog/using-bin2cpp
mkdir -p $OUTDIR/input_files/www/contact
mkdir -p $OUTDIR/generated_sources
mkdir -p $OUTDIR/saved_files
./testfilegenerator --size=1010 --fill=html --file=$OUTDIR/input_files/www/index.html
./testfilegenerator --size=1010 --fill=html --file=$OUTDIR/input_files/www/home/index.html
./testfilegenerator --size=1010 --fill=html --file=$OUTDIR/input_files/www/blog/index.html
./testfilegenerator --size=1010 --fill=html --file=$OUTDIR/input_files/www/blog/how-to-create-a-web-site/index.html
./testfilegenerator --size=1010 --fill=html --file=$OUTDIR/input_files/www/blog/using-bin2cpp/index.html
./testfilegenerator --size=1010 --fill=html --file=$OUTDIR/input_files/www/contact/index.html
./bin2cpp --noheader --dir=$OUTDIR/input_files --output=$OUTDIR/generated_sources --chunksize=200 --managerfile=FileManagerReportedPathDir.h --registerfile --namespace=testReportedPathDir --override

echo "SCRIPT GENERATE_TEST_FILES - END"
