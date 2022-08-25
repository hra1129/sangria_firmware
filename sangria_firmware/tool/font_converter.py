#!/usr/bin/env python3
# coding=utf-8

import sys
import re

try:
	from PIL import Image
except:
	print( "ERROR: Require PIL module. Please run 'pip3 install Pillow.'" )
	exit()

def convert( input_name, output_name ):
	try:
		img = Image.open( input_name )
	except:
		print( "ERROR: Cannot read the '%s'." % input_name )
		return

	img = img.convert( 'RGB' )
	with open( "%s.cpp" % output_name, 'wt' ) as file:
		file.write( '#include <cstdint>\n' )
		file.write( '\n' )
		file.write( 'int %s_width  = %d;\n' % ( output_name, img.width ) )
		file.write( 'int %s_height = %d;\n' % ( output_name, img.height ) )
		file.write( 'const uint8_t %s[] = {\n' % output_name )
		for c in range( 0, 96 ):
			file.write( '\t' )
			for y in range( 0, 8 ):
				d = 0
				for x in range( 0, 8 ):
					( r, g, b ) = img.getpixel( ( c * 8 + x, y ) )
					p = int((r + g + b) / 3)
					if p >= 128:
						d = (d << 1) | 1
					else:
						d = (d << 1) | 0
				file.write( '0x%02X, ' % d )
			if c == 96:
				file.write( "// DEL\n" )
			else:
				file.write( "// '%c'\n" % (c + 32) )
		file.write( '};\n' )
	print( "Success!!" )

def usage():
	print( "Usage> font_converter.py <image_file 768x8>" )

def main():
	if len( sys.argv ) < 2:
		usage()
		exit()
	output_name = re.sub( r'^(.*)\..*?$', r'\1', sys.argv[1] )
	print( "Input  name: %s" % sys.argv[1] )
	print( "Output name: %s" % output_name )
	convert( sys.argv[1], output_name )

if __name__ == "__main__":
	main()
