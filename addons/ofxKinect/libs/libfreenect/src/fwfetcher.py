#!/usr/bin/env python2

from __future__ import absolute_import, division, print_function

import hashlib
import io
import os
import struct
import sys
import time
import zipfile

try:
    # Python 3
    from urllib.request import Request, URLError, urlopen
except ImportError:
    # Python 2
    from urllib2 import Request, URLError, urlopen

# fwfetcher.py - a program to extract the Kinect audio firmware from an Xbox360
# system update.  This program includes substantial portions of extract360.py,
# which is copyright Rene Ladan and others as noted below and provided under
# the BSD 2-clause license.

"""Program to extract typical XBox 360 files.
   It can handle LIVE/PIRS, CON (partially), FMIM, and XUIZ files.

   What about CRA (aka .arc) files?  (Dead Rising demo)

   Copyright (c) 2007, 2008, Rene Ladan <r.c.ladan@gmail.com>, 2-claused BSD
   license. Portions from various contributors as mentioned in-source.

   Note that it dumps UTF-16 characters in text strings as-is.
"""


# Minor compatibility shim
if sys.version_info[0] < 3:
    input = raw_input
    range = xrange

###############################################################################

def check_size(fsize, minsize):
    """Ensure that the filesize is at least minsize bytes.

       @param fsize the filesize
       @param minsize the minimal file size
       @return fsize >= minsize
    """

    if fsize < minsize:
        print("Input file too small: %i instead of at least %i bytes." % (
            fsize, minsize))
    return fsize >= minsize

###############################################################################

def nice_open_file(filename):
    """Checks if the output file with the given name already exists,
       and if so, asks for overwrite permission.

       @param filename name of the output file to open
       @return overwrite permission
    """

    if os.path.isfile(filename):
        print(filename, "already exists, overwrite? (y/n)", end=' ')
        answer = input("")
        return len(answer) > 0 and answer[0] in ["Y", "y"]
    else:
        return True

###############################################################################

def nice_open_dir(dirname):
    """Checks if the output directory with the given name already exists,
       and if so, asks for overwrite permission.  This means that any file
       in that directory might be overwritten.

       @param dirname name of the output directory to open
       @return overwrite permission
    """

    if os.path.isdir(dirname):
        print(dirname, "already exists, ok to overwrite files in it? (y/n)",
              end=' ')
        answer = input("")
        return len(answer) > 0 and answer[0] in ["Y", "y"]
    else:
        return True

###############################################################################

def do_mkdir(dirname):
    """Version of os.mkdir() which does not throw an exception if the directory
       already exists.

       @param dirname name of the directory to create
    """

    try:
        os.mkdir(dirname)
    except OSError as e:
        if e.errno == 17:
            pass  # directory already exists

###############################################################################

def strip_blanks(instring):
    """Strip the leading and trailing blanks from the input string.
       Blanks are: 0x00 (only trailing) space \t \n \r \v \f 0xFF

       @param instring the input string
       @return stripped version of instring
    """

    rstr = instring.rstrip(b"\0 \t\n\r\v\f\377")
    return rstr.lstrip(b" \t\n\r\v\f\377")

###############################################################################

def open_info_file(infile):
    """Open the informational text file.
       The name is based on that of the input file.

       @param infile pointer to the input file
       @return pointer to the informational text file or None if there was no
               overwrite permission
    """

    txtname = os.path.basename(infile.name) + ".txt"
    if nice_open_file(txtname):
        print("Writing information file", txtname)
        txtfile = open(txtname, "w")
        return txtfile
    else:
        return None

###############################################################################

def dump_png(infile, pnglen, maxlen, pngid):
    """Dump the embedded PNG file from the archive file to an output file.

       @param infile pointer to the archive file
       @param pnglen size of the PNG file in bytes
       @param maxlen maximum size of the PNG file in bytes
       @param pngid indicates if this is the first or second PNG file.
    """

    # dump PNG icon
    if pnglen <= maxlen:
        outname = os.path.basename(infile.name) + "_" + pngid + ".png"
        if nice_open_file(outname):
            buf = infile.read(pnglen)
            print("Writing PNG file", outname)
            with open(outname, "wb") as outfile:
                print(buf, end=' ', file=outfile)
    else:
        print("PNG image %s too large (%i instead of maximal %i bytes), "
              "file not written." % (pngid, pnglen, maxlen))

###############################################################################

def dump_info(infile, txtfile, what):
    """Dumps the 9 information strings from the input file.

       @param infile pointer to the input file
       @param txtfile pointer to the resulting text file
       @param what indicates if the information consists of titles or
              descriptions
    """

    print("\n", what, ":", file=txtfile)
    for i in range(9):
        info = strip_blanks(infile.read(0x100))
        if len(info) > 0:
            print(lang[i], ":", info, file=txtfile)

###############################################################################

def mstime(intime):
    """Convert the time given in Microsoft format to a normal time tuple.

       @param intime the time in Microsoft format
       @return the time tuple
    """

    num_d = (intime & 0xFFFF0000) >> 16
    num_t = intime & 0x0000FFFF
    # format below is : year, month, day, hour, minute, second,
    #                   weekday (Monday), yearday (unused), DST flag (guess)
    return ((num_d >> 9) + 1980, (num_d >> 5) & 0x0F, num_d & 0x1F,
            (num_t & 0xFFFF) >> 11, (num_t >> 5) & 0x3F, (num_t & 0x1F) * 2,
            0, 0, -1)

###############################################################################

def do_utime(targetname, atime, mtime):
    """Set the access and update date/time of the target.
       Taken from tarfile.py (builtin lib)

       @param targetname name of the target
       @param atime the desired access date/time
       @param mtime the desired update date/time
    """

    if not hasattr(os, "utime"):
        return
    if not (sys.platform == "win32" and os.path.isdir(targetname)):
        # Using utime() on directories is not allowed on Win32 according to
        # msdn.microsoft.com
        os.utime(targetname,
                 (time.mktime(mstime(atime)), time.mktime(mstime(mtime))))

###############################################################################

def check_sha1(sha1, entry, infile, start, end):
    """Check the SHA1 value of the specified range of the input file.

       @param sha1 the reported SHA1 value
       @param entry the id of the hash
       @param infile the input file to check
       @param start the start position
       @param end the end position
       @return string reporting if the hash is correct
    """

    infile.seek(start)
    found_sha1 = hashlib.sha1(infile.read(end - start))
    found_digest = found_sha1.digest()
    # SHA1 hashes are 20 bytes (160 bits) long
    ret = "SHA1 " + hex(entry) + " "
    if found_digest == sha1:
        return ret + "ok (" + found_sha1.hexdigest() + ")"
    else:
        hexdig = ""
        for i in sha1:
            if ord(i) < 10:
                val = "0"
            else:
                val = ""
            val += hex(ord(i))[2:]
            hexdig += val
        return ret + "wrong (should be " + hexdig + " actual " + \
            found_sha1.hexdigest() + ")"

###############################################################################

def get_cluster(startclust, offset):
    """get the real starting cluster"""
    rst = 0
    # BEGIN wxPirs
    while startclust >= 170:
        startclust //= 170
        rst += (startclust + 1) * offset
    # END wxPirs
    return rst

###############################################################################

def fill_directory(infile, txtfile, contents, firstclust, makedir, start,
                   offset):
    """Fill the directory structure with the files contained in the archive.

       @param infile pointer to the archive
       @param txtfile pointer to the resulting information text file
       @param contents contains the directory information
       @param firstclust address of the starting cluster of the first file in
              infile (in 4kB blocks, minus start bytes)
       @param makedir flag if directory should be filled, useful if only return
              is wanted
       @param start start of directory data
       @param offset increment for calculating real starting cluster
    """

    # dictionary which holds the directory structure,
    # patch 0xFFFF is the 'root' directory.
    paths = {0xFFFF: ""}

    oldpathind = 0xFFFF  # initial path, speed up file/dir creation

    for i in range(0x1000 * firstclust // 64):
        cur = contents[i * 64:(i + 1) * 64]
        if ord(cur[40:41]) == 0:
            # if filename length is zero, we're done
            break
        (outname, namelen, clustsize1, val1, clustsize2, val2, startclust,
            val3) = struct.unpack("<40sBHBHBHB", cur[0:50])
        # sizes and starting cluster are 24 bits long
        clustsize1 += val1 << 16
        clustsize2 += val2 << 16
        startclust += val3 << 16
        (pathind, filelen, dati1, dati2) = struct.unpack(">HLLL", cur[50:64])

        if not makedir:
            continue

        nlen = namelen & ~0xC0
        if nlen < 1 or nlen > 40:
            print("Filename length (%i) out of range, skipping file." % nlen)
            continue
        outname = outname[0:nlen]  # strip trailing 0x00 from filename

        if txtfile is not None:
            if namelen & 0x80 == 0x80:
                print("Directory", end=' ', file=txtfile)
            else:
                print("File", end=' ', file=txtfile)
            print("name:", outname, file=txtfile)
            if namelen & 0x40 == 0x40:
                print("Bit 6 of namelen is set.", file=txtfile)

        if clustsize1 != clustsize2:
            print("Cluster sizes don't match (%i != %i), skipping file." % (
                clustsize1, clustsize2))
            continue
        if startclust < 1 and namelen & 0x80 == 0:
            print("Starting cluster must be 1 or greater, skipping file.")
            continue
        if filelen > 0x1000 * clustsize1:
            print("File length (%i) is greater than the size in clusters "
                  "(%i), skipping file." % (filelen, clustsize1))
            continue

        if pathind != oldpathind:
            # working directory changed
            for _ in range(paths[oldpathind].count("/")):
                os.chdir("..")  # go back to root directory
            os.chdir(paths[pathind])
            oldpathind = pathind
        if namelen & 0x80 == 0x80:
            # this is a directory
            paths[i] = paths[pathind] + outname + "/"
            do_mkdir(outname)
        else:
            # this is a file
            # space between files is set to 0x00
            adstart = startclust * 0x1000 + start
            if txtfile is not None:
                print("Starting: advertized", hex(adstart), file=txtfile)

            # block reading algorithm originally from wxPirs
            buf = b""
            while filelen > 0:
                realstart = adstart + get_cluster(startclust, offset)
                infile.seek(realstart)
                buf += infile.read(min(0x1000, filelen))
                startclust += 1
                adstart += 0x1000
                filelen -= 0x1000
            with open(outname, "wb") as outfile:
                outfile.write(buf)

        do_utime(outname, dati2, dati1)

    # pop directory
    for _ in range(paths[oldpathind].count("/")):
        os.chdir("..")

###############################################################################

def write_common_part(infile, txtfile, png2stop, start):
    """Writes out the common part of PIRS/LIVE and CON files.

       @param infile pointer to the PIRS/LIVE or CON file
       @param txtfile pointer to the resulting text file
       @param png2stop location where the second PNG image stops
                  (PIRS/LIVE : 0xB000, CON : 0xA000)
       @param start start of directory data, from wxPirs
    """

    infile.seek(0x32C)
    # xbox180 : SHA1 hash of 0x0344-0xB000,
    # CON : 0x0344 - 0xA000 (i.e. png2stop)
    mhash = infile.read(20)
    (mentry_id, content_type) = struct.unpack(">LL", infile.read(8))

    if txtfile is not None:
        print("\nMaster SHA1 hash :",
              check_sha1(mhash, mentry_id, infile, 0x0344, png2stop),
              file=txtfile)
        print("\nContent type", hex(content_type), ":", end=' ', file=txtfile)
        # content type list partially from V1kt0R
        # su20076000_00000000 has type 0x000b0000,
        # i.e. "Full game demo" & "Theme" ...
        if content_type == 0:
            print("(no type)", file=txtfile)
        elif content_type & 0x00000001:
            print("Game save", file=txtfile)
        elif content_type & 0x00000002:
            print("Game add-on", file=txtfile)
        elif content_type & 0x00030000:
            print("Theme", file=txtfile)
        elif content_type & 0x00090000:
            print("Video clip", file=txtfile)
        elif content_type & 0x000C0000:
            print("Game trailer", file=txtfile)
        elif content_type & 0x000D0000:
            print("XBox Live Arcade", file=txtfile)
        elif content_type & 0x00010000:
            print("Gamer profile", file=txtfile)
        elif content_type & 0x00020000:
            print("Gamer picture", file=txtfile)
        elif content_type & 0x00040000:
            print("System update", file=txtfile)
        elif content_type & 0x00080000:
            print("Full game demo", file=txtfile)
        else:
            print("(unknown)", file=txtfile)

        print("\nDirectory data at (hex)", hex(start), file=txtfile)
        infile.seek(0x410)
        dump_info(infile, txtfile, "Titles")
        dump_info(infile, txtfile, "Descriptions")
        print("\nPublisher:", strip_blanks(infile.read(0x80)), "\n",
              file=txtfile)
        print("\nFilename:", strip_blanks(infile.read(0x80)), "\n",
              file=txtfile)
    infile.seek(0x1710)
    (val1, png1len, png2len) = struct.unpack(">HLL", infile.read(10))
    if txtfile is not None:
        print("Value:", val1, file=txtfile)

    if png1len > 0:
        dump_png(infile, png1len, 0x571A - 0x171A, "1")

    if png2len > 0:
        infile.seek(0x571A)
        dump_png(infile, png2len, png2stop - 0x571A, "2")

    # entries are 64 bytes long
    # unused entries are set to 0x00
    infile.seek(start + 0x2F)
    (firstclust, ) = struct.unpack("<H", infile.read(2))

    infile.seek(start)
    buf = infile.read(0x1000 * firstclust)

    outname = os.path.basename(infile.name) + ".dir"
    makedir = nice_open_dir(outname)
    if makedir:
        print("Creating and filling content directory", outname)
        do_mkdir(outname)
        os.chdir(outname)
    if png2stop == 0xB000 and start == 0xC000:
        offset = 0x1000
    else:
        offset = 0x2000
    fill_directory(infile, txtfile, buf, firstclust, makedir, start, offset)

    # table of SHA1 hashes of payload
    if txtfile is not None:
        print(file=txtfile)
        infile.seek(png2stop)
        buf = infile.read(start - png2stop)
        numempty = 0
        for i in range(len(buf) // 24):
            entry = buf[i * 24: i * 24 + 24]
            if entry.count(b"\0") < 24:
                if numempty > 0:
                    print("\nEmpty entries:", numempty, file=txtfile)
                    numempty = 0
                print("Hash (hex):", end=' ', file=txtfile)
                for j in range(20):
                    print(hex(ord(entry[j:j + 1])), end=' ', file=txtfile)
                (j, ) = struct.unpack(">L", entry[20:24])
                print("\nEntry id:", hex(j), file=txtfile)
            else:
                numempty += 1

        print("\nTrailing data (hex):", end=' ', file=txtfile)
        for i in range(len(buf) - (len(buf) % 24), len(buf)):
            print(hex(ord(buf[i:i + 1])), end=' ', file=txtfile)
        print(file=txtfile)

        txtfile.close()

###############################################################################

def handle_live_pirs(infile, fsize):
    """LIVE and PIRS files are archive files.
       They contain a certificate, payload, SHA1 checksums,
       2 icons, textual information, and the files themselves.

       @param infile pointer to the archive file
       @param fsize size of infile
    """

    print("Handling LIVE/PIRS file.")

    if not check_size(fsize, 0xD000):
        return

    txtfile = open_info_file(infile)
    if txtfile is not None:
        print("Certificate (hex):", end=' ', file=txtfile)
        cert = infile.read(0x100)
        for i in range(len(cert)):
            print(hex(ord(cert[i:i + 1])), end=' ', file=txtfile)

        print("\n\nData (hex):", end=' ', file=txtfile)
        data = infile.read(0x228)
        for i in range(len(data)):
            print(hex(ord(data[i:i + 1])), end=' ', file=txtfile)
        print(file=txtfile)

    # BEGIN wxPirs
    infile.seek(0xC032)  # originally 4 bytes at 0xC030
    (pathind, ) = struct.unpack(">H", infile.read(2))
    if pathind == 0xFFFF:
        start = 0xC000
    else:
        start = 0xD000
    # END wxPirs
    write_common_part(infile, txtfile, 0xB000, start)

###############################################################################

# End of code taken from extract360.py.

def getFileOrURL(filename, url):
    # Check if a file named filename exists on disk.
    # If so, return its contents.  If not, download it, save it, and return its
    # contents.
    try:
        with open(filename, 'rb') as f:
            print("Found", filename, "cached on disk, using local copy")
            retval = f.read()
        return retval
    except IOError:
        pass
    print("Downloading", filename, "from", url)
    req = Request(url, headers={'User-Agent': 'Mozilla/5.0'})
    try:
        response = urlopen(req)
    except URLError as e:
        if hasattr(e, 'reason'):
            print("Failed to reach download server.  Reason:", e.reason)
        elif hasattr(e, 'code'):
            print("The server couldn't fulfill the request.  Error code:",
                  e.code)
    print("Reading response...")
    retval = response.read()
    # Save downloaded file to disk
    with open(filename, "wb") as f:
        f.write(retval)
    print("done, saved to", filename)
    return retval

def extractPirsFromZip(systemupdate):
    print("Extracting $SystemUpdate/FFFE07DF00000001 from system update "
          "file...")
    updatefile = io.BytesIO(systemupdate)
    with zipfile.ZipFile(updatefile) as z:
        # print(z.namelist())
        pirs = z.open("$SystemUpdate/FFFE07DF00000001").read()
    print("done.")
    return pirs

if __name__ == "__main__":
    target = "audios.bin"
    if len(sys.argv) == 2:
        target = sys.argv[1]
    if not os.path.isfile(target):
        fw = getFileOrURL("SystemUpdate.zip",
                          "http://www.xbox.com/system-update-usb")
        pirs = extractPirsFromZip(fw)

        lang = ["English", "Japanese", "German", "French", "Spanish",
                "Italian", "Korean", "Chinese", "Portuguese"]
        bio = io.BytesIO(pirs)
        basename = "FFFE07DF00000001"
        bio.name = basename
        pwd = os.getcwd()
        handle_live_pirs(bio, len(pirs) - 4)

        os.chdir(pwd)
        print("Moving audios.bin to current folder")
        os.rename(os.path.join(basename + ".dir", "audios.bin"), target)

        print("Cleaning up")
        os.unlink(basename + ".txt")
        for root, dirs, files in os.walk(basename + ".dir"):
            for name in files:
                os.remove(os.path.join(root, name))
            for name in dirs:
                os.rmdir(os.path.join(root, name))
            os.rmdir(root)
        os.unlink("SystemUpdate.zip")
        print("Done!")
    else:
        print("Already have audios.bin")
