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

#include <libcorpus2/io/rft.h>

#include <libpwrutils/foreach.h>

#include <boost/algorithm/string.hpp>
#include <boost/make_shared.hpp>

namespace Corpus2 {

bool RftWriter::registered = TokenWriter::register_writer<RftWriter>(
		"rft", "");

RftWriter::RftWriter(std::ostream& os, const Tagset& tagset,
		const string_range_vector& params)
	: TokenWriter(os, tagset, params), warn_on_no_lexemes_(true)
{
	foreach (const string_range& param, params) {
		std::string p = boost::copy_range<std::string>(param);
		if (p == "nowarn") {
			warn_on_no_lexemes_ = false;
		}
	}
}

void RftWriter::write_token(const Token& t)
{
	os() << t.orth_utf8() << "\t";
	if (t.lexemes().empty()) {
		if (warn_on_no_lexemes_) {
			std::cerr << "No lexemes for token!";
		}
	} else {
		const Lexeme& pref = t.get_preferred_lexeme(tagset());
		std::string tag_str = tagset().tag_to_no_opt_string(pref.tag());
		os() << boost::algorithm::replace_all_copy(tag_str, ":", ".");
	}
	os() << "\n";
}

void RftWriter::write_sentence(const Sentence& s)
{
	foreach (const Token* t, s.tokens()) {
		write_token(*t);
	}
	os() << "\n";
}

void RftWriter::write_chunk(const Chunk& c)
{
	foreach (const Sentence::ConstPtr& s, c.sentences()) {
		write_sentence(*s);
	}
}

RftReader::RftReader(const Tagset& tagset, std::istream& is, bool disamb)
	: BufferedSentenceReader(tagset), is_(is), disamb_(disamb)
{
}

Sentence::Ptr RftReader::actual_next_sentence()
{
	std::string line;
	Sentence::Ptr s;
	while (is().good()) {
		std::getline(is(), line);
		if (line.empty()) {
			return s;
		} else {
			size_t tab = line.find('\t');
			if (tab == line.npos || tab == 0 || (tab == line.size() - 1)) {
				std::cerr << "Invalid line: " << line << "\n";
			} else {
				std::string orth = line.substr(0, tab);
				std::string tag_string = line.substr(tab + 1);
				boost::algorithm::replace_all(tag_string, ".", ":");
				Tag tag = tagset().parse_simple_tag(tag_string, false);
				Token* t = new Token();
				t->set_orth(UnicodeString::fromUTF8(orth));
				t->set_wa(PwrNlp::Whitespace::Space);
				if (!s) {
					s = boost::make_shared<Sentence>();
					t->set_wa(PwrNlp::Whitespace::Newline);
				}
				t->add_lexeme(Lexeme(t->orth(), tag));
				if (disamb_) {
					t->lexemes().back().set_disamb(true);
				}
				s->append(t);
			}
		}
	}
	return s;
}


} /* end ns Corpus2 */
