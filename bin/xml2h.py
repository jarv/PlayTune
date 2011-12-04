#!/usr/bin/python
# -*- coding: utf-8 -*-

from BeautifulSoup import BeautifulStoneSoup
from collections import defaultdict
from os.path import splitext, basename
import re, sys

if len(sys.argv) < 2:
    print "Usage: " + sys.argv[0] + " <musicxml> <header filename (opt)>"
    exit()
fname = sys.argv[1]
fheader = None
if len(sys.argv) == 3:
    fheader = sys.argv[2]

freq = {
        "1" : 1000000,
        "2" : 8000000,
        "3" : 16000000,
        }

freq_map = {
    # Name  : Freq. (Hz) 
    u"C8 Eighth octave" : 4186.01,
    u"B7" : 3951.07,
    u"A♯7/B♭7" : 3729.31,
    u"A7" : 3520,
    u"G♯7/A♭7" : 3322.44,
    u"G7" : 3135.96,
    u"F♯7/G♭7" : 2959.96,
    u"F7" : 2793.83,
    u"E7" : 2637.02,
    u"D♯7/E♭7" : 2489.02,
    u"D7" : 2349.32,
    u"C♯7/D♭7" : 2217.46,
    u"C7 Double high C" : 2093,
    u"B6" : 1975.53,
    u"A♯6/B♭6" : 1864.66,
    u"A6" : 1760,
    u"G♯6/A♭6" : 1661.22,
    u"G6" : 1567.98,
    u"F♯6/G♭6" : 1479.98,
    u"F6" : 1396.91,
    u"E6" : 1318.51,
    u"D♯6/E♭6" : 1244.51,
    u"D6" : 1174.66,
    u"C♯6/D♭6" : 1108.73,
    u"C6 Soprano C (High C)" : 1046.5,
    u"B5" : 987.767,
    u"A♯5/B♭5" : 932.328,
    u"A5" : 880,
    u"G♯5/A♭5" : 830.609,
    u"G5" : 783.991,
    u"F♯5/G♭5" : 739.989,
    u"F5/E♯5" : 698.456,
    u"E5/F♭5" : 659.255,
    u"D♯5/E♭5" : 622.254,
    u"D5" : 587.33,
    u"C♯5/D♭5" : 554.365,
    u"C5 Tenor C" : 523.251,
    u"C♭5" : 493.883,
    u"B4" : 493.883,
    u"A♯4/B♭4" : 466.164,
    u"A4 A440" : 440,
    u"G♯4/A♭4" : 415.305,
    u"G4" : 391.995,
    u"F♯4/G♭4" : 369.994,
    u"F4" : 349.228,
    u"E4/F♭4" : 329.628,
    u"D♯4/E♭4" : 311.127,
    u"D4" : 293.665,
    u"C♯4/D♭4" : 277.183,
    u"C4 Middle C" : 261.626,
    u"C♭4" : 246.942,
    u"B♯3" : 261.626,
    u"B3" : 246.942,
    u"A♯3/B♭3" : 233.082,
    u"A3" : 220,
    u"G♯3/A♭3" : 207.652,
    u"G3" : 195.998,
    u"F♯3/G♭3" : 184.997,
    u"F3" : 174.614,
    u"E3" : 164.814,
    u"D♯3/E♭3" : 155.563,
    u"D3" : 146.832,
    u"C♯3/D♭3" : 138.591,
    u"C3 Low C" : 130.813,
    u"B♯2" : 130.813,
    u"B2/C♭3" : 123.471,
    u"A♯2/B♭2" : 116.541,
    u"A2" : 110,
    u"G♯2/A♭2" : 103.826,
    u"G2" : 97.9989,
    u"F♯2/G♭2" : 92.4986,
    u"F2" : 87.3071,
    u"E♯2" : 87.3071,
    u"E2" : 82.4069,
    u"D♯2/E♭2" : 77.7817,
    u"D2" : 73.4162,
    u"C♯2/D♭2" : 69.2957,
    u"C2 Deep C" : 65.4064,
    u"B1" : 61.7354,
    u"A♯1/B♭1" : 58.2705,
    u"A1" : 55,
    u"G♯1/A♭1" : 51.9131,
    u"G1" : 48.9994,
    u"F♯1/G♭1" : 46.2493,
    u"F1" : 43.6535,
    u"E1" : 41.2034,
    u"D♯1/E♭1" : 38.8909,
    u"D1" : 36.7081,
    u"C♯1/D♭1" : 34.6478,
    u"C1 Pedal C" : 32.7032,
    u"B0" : 30.8677,
    u"A♯0/B♭0" : 29.1352,
    u"A0 Double Pedal A" : 27.5,
}

def raw_input_def(prompt, default):
    while True:
        data = raw_input(prompt + ' [' + default + ']: ')
        if not (data == '' and default == ''):
            break

    if data == '':
        return default
    else:
        return data

def process_part(p, fq):

    data = {}
    prescale = {
        "9" : 1024,
        "8" : 512,
        "7" : 256,
        "6" : 128,
        "5" : 64,
        "4" : 32,
        "3" : 16,
        "2" : 8,
        "1" : 1,
        }

    data=[]
    data_starts=[]
    num_voices = 0  

    for measure in soup('part')[p].findAll('measure'):
        mdata = defaultdict(list)
        mdata_starts = defaultdict(list)
        # keeps track of the previous note so we can
        # tell if two notes are tied together and
        # mark them contiguous
        prev_note = None

        for note in measure.findAll('note'):
            if note.chord:
                next  # skip chords

            # this is a poor-man's musicxml parser :(
            # some validation checks before we parse
            # every note has one and only one 
            # duration and voice
            if not (len(note.duration.contents) == 1 and \
                    len(note.voice.contents) == 1):
                raise ValueError
            v = note.voice.contents[0] 
            d = note.duration.contents[0]
            if (note.rest):
                # fill the note data with 0s
                mdata[v].extend([0 for x in range(0,int(d))])
                # mark the beginning of the rest 
                mdata_starts[v].append(1)
                mdata_starts[v].extend([ 0 for x in range(0,int(d)-1)]) 
            else:
                # every non-rest must have one and only one
                # step and octave
                if not (len(note.step.contents) == 1 and \
                        len(note.octave.contents) ==1):
                    raise ValueError
                # start of a new note
                notestr = unicode(note.step.contents[0])
                if note.alter:
                    if note.alter.contents[0] == "-1":
                        notestr += u"♭"
                    elif note.alter.contents[0] == "1":
                        notestr += u"♯"
                    else:
                        raise ValueError
                notestr += note.octave.contents[0]
                # lookup a corresponding frequency
                # for the note in the mapping dict
                lookup = False 
                for i in freq_map.keys():
                    if re.search(notestr,i):
                        lookup = freq_map[i]
                        break
                if not lookup:
                    sys.exit("Unable to find match for " + notestr)

                prev_note = lookup
                mdata[v].extend([ lookup for x in range(0,int(d))])

                # mark the beginning of the rest or note
                # and fill the rest of the start data
                # with 0s (unless it's tied)
                if (prev_note == lookup and \
                        note.find('tie', attrs={'type':'stop'})):
                    mdata_starts[v].extend([ 0 for x in range(0,int(d))])
                else:
                    mdata_starts[v].append(1)
                    mdata_starts[v].extend([ 0 for x in range(0,int(d)-1)]) 


        # update the number of voices
        if num_voices < len(mdata.keys()):
            num_voices = len(mdata.keys())

        # add the measure data to the frequency array
        # the first voice will take precedence if the 
        # different voices are merged. 
        merged = []
        merged_starts = []
        for voice in sorted(mdata.keys()):
            for index in range(0, len(mdata[voice])):
                if len(merged) == index:
                    merged.append( mdata[voice][index] )
                    merged_starts.append( mdata_starts[voice][index] )
                elif len(merged) > index:
                    # only overwrite the merged data if the 
                    # value at the index is 0
                    if not merged[index]:
                        merged[index] = mdata[voice][index]
                        merged_starts[index] = mdata_starts[voice][index]
                else:
                    raise ValueError
        data.extend(merged)
        data_starts.extend(merged_starts)


    # filter prescales removing ones that give values > 255
    for k in sorted(prescale.keys()):
        ps = prescale[k]
        tmp = [int(round((fq - (2*ps*i))/(2*ps*i),0)) if i else 0 for i in data]
        if max(tmp) > 255:
            prescale.pop(k)

    if not len(prescale.keys()):
        sys.exit("ERROR: all prescales invalid")

    for i in sorted(prescale.keys()):
        print "  %2s) %6s" % (i, str(prescale[i]))


    ps = prescale[ raw_input_def("Pick prescale", str(min(prescale.keys())))]
    t_data = [int(round((fq - (2*ps*i))/(2*ps*i),0)) if i else 0 for i in data]

    notes = []
    delays = []
    for pos in range(0, len(t_data)):
        if data_starts[pos]:
            notes.append(t_data[pos])
            delays.append(1)
        else:
            delays[-1] += 1

    rdata = {}
    rdata['prescale'] = ps
    rdata['notes'] = notes
    rdata['delays'] = delays
    rdata['freq'] = fq
    rdata['voices'] = num_voices
    return rdata


f = open(fname)
data = f.read()
f.close()

soup = BeautifulStoneSoup(data)
data = []
num_parts = len(soup('part'))

if num_parts < 1:
    print >> sys.stderr, "ERROR: no parts found in file " + fname
    exit()

for i in sorted(freq.keys()):
    print "%2s) %10s" % (i, str(freq[i]))
fq = freq[raw_input_def("Pick Frequency (Hz)", "3")]


part_select = 0
d = []
print "Found " + str(num_parts) + " parts in " + fname

while part_select < num_parts:
    p = raw_input_def("Choose a part to process ('q' to quit) [0-" + str(num_parts-1) + "]", str(part_select))
    if p == 'q':
        break
    else:
        d.append(process_part(int(p),fq))
    part_select += 1

# create header file
name = basename(splitext(fname)[0])
name = re.sub(re.compile("[-]"),"_",name)

hdefines = "#ifndef " + name.upper() + "\n" + \
        "#define " + name.upper() + "\n" + \
        "#include <stdint.h>\n" + \
        "#include <avr/pgmspace.h>\n\n"

hcomments = ""
harrays = ""

for cnt in range(0,len(d)):
    p = d[cnt]
    scnt = str(cnt)
    hdefines += "#define "+ name.upper() + scnt+" "+str(p['prescale'])+","+\
            name + scnt+ "_notes,"+ name + scnt+\
            "_delays,sizeof("+ name + scnt+"_notes)/sizeof(*("+ name + \
            scnt + "_notes)),sizeof("+ name + scnt+\
            "_delays)/sizeof(*("+ name + scnt+ \
            "_delays))\n"

    hcomments += "\n    Part: " + scnt + \
             "\n    Freq: " + str(p['freq']) + \
             "\n    Number of voices: " + str(p['voices']) + \
             "\n    Note length: " + str(len(p['notes'])) + \
             "\n    Delay length: " + str(len(p['delays']))  + \
             "\n"


    harrays += "const uint8_t "+ name + scnt+"_notes[] PROGMEM = {\n" + \
            ",".join( [str(x) for x in p['notes']]) + \
            "\n};\n"
    harrays += "const uint8_t "+ name + scnt+"_delays[] PROGMEM = {\n" + \
            ",".join( [str(x) for x in p['delays']]) + \
            "\n};\n\n"



if (fheader):
    f = open(fheader,'w')
    f.write( hdefines + \
     "/*\n" + hcomments + "\n*/\n" + \
     harrays + "#endif\n")
    f.close
else:
    print hdefines
    print "/*\n" + hcomments + "\n*/\n"
    print harrays + "#endif\n"


