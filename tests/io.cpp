/*
    Copyright (C) 2010 Tomasz Śniatowski, Adam Radziszewski
    Part of the libcorpus2 project

    This program is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the Free
Software Foundation; either version 3 of the License, or (at your option)
any later version.

    This program is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.

    See the LICENSE and COPYING files for more details.
*/

#include <boost/test/unit_test.hpp>
#include <set>
#include <libpwrutils/foreach.h>
#include <libpwrutils/bitset.h>
#include <libcorpus2/tagsetmanager.h>
#include <libcorpus2/io/xcesreader.h>
#include <libcorpus2/io/writer.h>

namespace {
static char swiatopoglad[] =
"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
"<!DOCTYPE cesAna SYSTEM \"xcesAnaIPI.dtd\">\n"
"<cesAna xmlns:xlink=\"http://www.w3.org/1999/xlink\" version=\"1.0\" type=\"lex disamb\">\n"
"<chunkList>\n"
"<chunk id=\"ch51\" type=\"tok\">\n"
"<chunk type=\"s\">\n"
"<tok>\n"
"<orth>Uważam</orth>\n"
"<lex disamb=\"1\"><base>uważać</base><ctag>fin:sg:pri:imperf</ctag></lex>\n"
"</tok>\n"
"<ns/>\n"
"<tok>\n"
"<orth>,</orth>\n"
"<lex disamb=\"1\"><base>,</base><ctag>interp</ctag></lex>\n"
"</tok>\n"
"<tok>\n"
"<orth>że</orth>\n"
"<lex disamb=\"1\"><base>że</base><ctag>conj</ctag></lex>\n"
"</tok>\n"
"<tok>\n"
"<orth>światopogląd</orth>\n"
"<lex><base>światopogląd</base><ctag>subst:sg:acc:m3</ctag></lex>\n"
"<lex disamb=\"1\"><base>światopogląd</base><ctag>subst:sg:nom:m3</ctag></lex>\n"
"</tok>\n"
"</chunk>\n"
"</chunk>\n"
"</chunkList>\n"
"</cesAna>\n"
;
}

BOOST_AUTO_TEST_SUITE( io )

BOOST_AUTO_TEST_CASE( iobase )
{
	const Corpus2::Tagset& tagset = Corpus2::get_named_tagset("kipi");
	std::stringstream ssin;
	ssin << swiatopoglad;
	Corpus2::XcesReader xr(tagset, ssin);
	boost::shared_ptr<Corpus2::Chunk> chunk = xr.get_next_chunk();
	std::stringstream ss;
	boost::shared_ptr<Corpus2::TokenWriter> w(Corpus2::TokenWriter::create("xces,flat", ss, tagset));
	w->write_chunk(*chunk);
	w->finish();
	BOOST_CHECK_EQUAL(ss.str(), swiatopoglad);
}

BOOST_AUTO_TEST_SUITE_END();