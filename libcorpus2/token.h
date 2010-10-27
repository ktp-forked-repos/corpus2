#ifndef LIBCORPUS2_TOKEN_H
#define LIBCORPUS2_TOKEN_H

#include <libcorpus2/lexeme.h>
#include <libcorpus2/tagset.h>

#include <libpwrutils/util.h>
#include <libpwrutils/whitespace.h>

#include <unicode/unistr.h>
#include <string>
#include <vector>

namespace Corpus2 {

/**
 * A single token with morphological analyses.
 *
 * The orth is stored as an ICU UnicodeString, the token is also defined
 * by the preceeding whitespace measuere, and most importantly by the set
 * of possible interpretations stored as lexemes.
 */
class Token
	: boost::equality_comparable<Token>
{
public:
	/// Creates an empty Token
	Token();

	/// Create a Token with the given orth and whitespace amount
	Token(const UnicodeString& orth, PwrNlp::Whitespace::Enum wa);

	/// Create a duplicate Token
	Token* clone() const;

	/// Orth getter
	const UnicodeString& orth() const {
		return orth_;
	}

	/// Orth getter (UTF-8)
	std::string orth_utf8() const {
		return PwrNlp::to_utf8(orth_);
	}

	/// Orth setter
	void set_orth(const UnicodeString& orth) {
		orth_ = orth;
	}

	/// WA getter
	const PwrNlp::Whitespace::Enum& wa() const {
		return wa_;
	}

	/// WA setter
	void set_wa(const PwrNlp::Whitespace::Enum& wa) {
		wa_ = wa;
	}

	/// Lexemes getter
	const std::vector<Lexeme>& lexemes() const {
		return lexemes_;
	}

	/// Lexemes nonconst getter
	std::vector<Lexeme>& lexemes() {
		return lexemes_;
	}

	/// Lexemes adder
	void add_lexeme(const Lexeme& lex) {
		lexemes_.push_back(lex);
	}

	/**
	 * Get the 'best' lexeme available, with best meaning disambiguated if
	 * possible, and when there is more than one candidate, some form of
	 * sorting is performed to always return the same lexeme.
	 */
	const Lexeme& get_preferred_lexeme(const Tagset& tagset) const;

	/// Returns the index of the best lexeme, @see get_best_lexeme
	size_t get_preferred_lexeme_index(const Tagset& tagset) const;

	/// Clear all lexemes and add one lexeme with lemma=orth and an ign tag
	void make_ign(const Tagset& tagset);

	/// Add a lexeme with lemma=orth and an ign tag
	void add_ign(const Tagset& tagset);

	/// Equality operator (!= is provided by boost template magic)
	bool operator==(const Token& other) const;

	/**
	 * Check for duplicate lexemes.
	 *
	 * Returns true if there are duplicate lexemes, false otherwise.
	 */
	bool check_duplicate_lexemes() const;

	/**
	 * Remove duplicate lexemes.
	 *
	 * Removes any duplicate lexemes and returns true if there were any.
	 * Ordering of the lexemes can change.
	 */
	bool remove_duplicate_lexemes();

	/**
	 * Check if all the lexemes of the token have the given POS, and and
	 * the token orth caselessly matches the given orth.
	 *
	 * If the given orth is empty, it is not checked.
	 * If the given pos idx is -1, it is not checked.
	 *
	 * @returns true if the orth and lexemes pass the check, false
	 * otherwise
	 */
	bool orth_pos_match(mask_t pos, const UnicodeString& orth) const;

private:
	/// The orth (actual encountered form)
	//boost::flyweight<UnicodeString> orth_;
	UnicodeString orth_;

	/// The preceeding whitespace amount marker
	PwrNlp::Whitespace::Enum wa_;

	/// The possible lexemes
	std::vector<Lexeme> lexemes_;
};

} /* end ns Corpus2 */

#endif // LIBCORPUS2_TOKEN_H
