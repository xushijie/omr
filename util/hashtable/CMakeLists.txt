###############################################################################
#
# (c) Copyright IBM Corp. 2017
#
#  This program and the accompanying materials are made available
#  under the terms of the Eclipse Public License v1.0 and
#  Apache License v2.0 which accompanies this distribution.
#
#      The Eclipse Public License is available at
#      http://www.eclipse.org/legal/epl-v10.html
#
#      The Apache License v2.0 is available at
#      http://www.opensource.org/licenses/apache2.0.php
#
# Contributors:
#    Multiple authors (IBM Corp.) - initial implementation and documentation
###############################################################################


#TODO need to port following makefile snippet
#ifeq ($(OMR_TOOLCHAIN),gcc)
#    MODULE_CFLAGS += -Wno-unused-value
#endif

add_tracegen(hashtable.tdf)

add_library(j9hashtable STATIC
	hash.c
	hashtable.c
	ut_hashtable.c
)

target_include_directories(j9hashtable
	PUBLIC
		.
)

target_link_libraries(j9hashtable
	PUBLIC
		j9avl
		j9pool
		omrutil
)
