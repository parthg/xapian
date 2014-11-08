/** @file snipperinternal.h
 * @brief Internals
 */
/* Copyright (C) 2012 Mihai Bivol
 * Copyright (C) 2014 Olly Betts
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301
 * USA
 */

#ifndef XAPIAN_INCLUDED_SNIPPERINTERNAL_H
#define XAPIAN_INCLUDED_SNIPPERINTERNAL_H

#include <xapian/enquire.h>
#include <xapian/snipper.h>
#include <xapian/stem.h>

#include <map>
#include <string>
#include <vector>

namespace Xapian {

class Snipper::Internal : public Xapian::Internal::intrusive_base {

    private:
	/** Checks if a term is marked as stemmed. */
	bool is_stemmed(const std::string & term);

	/// Copy not allowed
	Internal(const Internal &);
	/// Assignment not allowed
	void operator=(const Internal &);

    public:
	typedef int rm_docid;

	/** Holds information about a document in the relevance model.*/
	struct RMDocumentInfo {
	    /** ID in the relevance model */
	    rm_docid rm_id;
	    /** Document size in terms */
	    int document_size;
	    /** Weight of the document */
	    double weight;

	    RMDocumentInfo(rm_docid rm_id_, int document_size_, double weight_) :
		rm_id(rm_id_),
		document_size(document_size_),
		weight(weight_) { }
	};

	/** Holds information about a term in a document */
	struct TermDocInfo {
	    /** Relevance model document id.*/
	    rm_docid docid;
	    /** Frequency of term in document */
	    Xapian::termcount freq;

	    TermDocInfo(rm_docid docid_, Xapian::termcount freq_) :
		docid(docid_),
		freq(freq_) { }
	};

	/** Holds information about a term in the relevance model */
	struct RMTermInfo {
	    /** Documents that index the term in relevance model */
	    std::vector<TermDocInfo> indexed_docs_freq;
	    /** Occurrence in collection */
	    int coll_occurrence;

	    RMTermInfo() : coll_occurrence(0) { }
	};

	/** Holds information about a term and its position in a document */
	struct TermPositionInfo {
	    std::string term;
	    Xapian::termpos position;

	    TermPositionInfo(std::string term_, Xapian::termpos position_) :
		term(term_),
		position(position_) { }

	    bool operator < (const TermPositionInfo & other) const
	    {
		return position < other.position;
	    }
	};

	/** Stemmer used for generating text terms */
	Stem stemmer;

	/** Relevance Model documents. */
	std::vector<RMDocumentInfo> rm_documents;

	/** Relevance model data for each term */
	std::map<std::string, RMTermInfo> rm_term_data;

	/** Relevance model collection size */
	Xapian::doccount rm_coll_size;

	/** Relevance model total document weight */
	double rm_total_weight;

	Internal() : rm_coll_size(0),
		     rm_total_weight(0) { }

	/** Return snippet generated from text using the precalculated relevance model */
	std::string generate_snippet(const std::string & text,
				     size_t length,
				     unsigned int window_size,
				     double smoothing);

	/** Calculate relevance model based on a MSet.
	 *
	 *  @param mset		The MSet to base the model on
	 *  @param rm_docno	How many documents to use from @a mset
	 */
	void calculate_rm(const MSet & mset, Xapian::doccount rm_docno);
};

}

#endif /* XAPIAN_INCLUDED_SNIPPERINTERNAL_H */
