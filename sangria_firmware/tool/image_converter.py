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
		file.write( 'uint8_t %s[] = {\n' % output_name )
		line_count = 0
		x = 0
		y = 0
		for i in range( 0, img.width * img.height ):
			if (line_count % 16) == 0:
				file.write( '\t' )
			( r, g, b ) = img.getpixel( ( x, y ) )
			p = int((r + g + b) / 3)
			file.write( '0x%02X, ' % p )
			if (line_count % 16) == 15:
				file.write( '\n' )
			x = x + 1
			if x >= img.width:
				x = 0
				y = y + 1
			line_count = line_count + 1
		file.write( '};\n' )
	print( "Success!!" )

def usage():
	print( "Usage> image_converter.py <image_file>" )

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
