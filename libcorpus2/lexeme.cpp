#include <libcorpus2/lexeme.h>

namespace Corpus2 {

Lexeme::Lexeme()
	: lemma_(), tag_(), disamb_(false)
{
}

Lexeme::Lexeme(const UnicodeString &lemma, const Tag &tag)
	: lemma_(lemma), tag_(tag), disamb_(false)
{
}

Lexeme Lexeme::create(const UnicodeString& lemma, const Tag& tag)
{
	return Lexeme(lemma, tag);
}

bool Lexeme::is_null() const
{
	return lemma().length() == 0 || tag().is_null();
}

bool Lexeme::operator<(const Lexeme& other) const
{
	return lemma_ < other.lemma_
			|| (lemma_ == other.lemma_
				&& (tag_ < other.tag_
					|| (tag_ == other.tag_
						&& disamb_ && !other.disamb_)));
}

bool Lexeme::operator==(const Lexeme& other) const
{
	return lemma_ == other.lemma_ && tag_ == other.tag_ &&
			disamb_ == other.disamb_;
}

bool Lexeme::DisamblessComparator::operator()(const Lexeme& l, const Lexeme& other) const
{
	return l.lemma_ == other.lemma_	&& l.tag_ == other.tag_;
}

} /* end ns Corpus2 */
