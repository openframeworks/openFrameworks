// copyright (c) openFrameworks team 2010-2017
// copyright (c) Damian Stewart 2007-2009
#pragma once

#include "ofxOscMessage.h"

/// \class ofxOscBundle
/// \brief an OSC bundle of ofxOscMessages and/or other ofxOscBundles
class ofxOscBundle{
public:

	ofxOscBundle() {}
	ofxOscBundle(const ofxOscBundle& other);
	ofxOscBundle& operator=(const ofxOscBundle &other);
	/// for operator= and copy constructor
	ofxOscBundle& copy(const ofxOscBundle &other);

	/// clear bundle & message contents
	void clear();

	/// add another bundle to the bundle
	void addBundle(const ofxOscBundle &element);

	/// add a message to the bundle
	void addMessage(const ofxOscMessage &message);

	/// \return the current bundle count
	int getBundleCount() const;

	/// \return the current message count
	int getMessageCount() const;

	/// \return the bundle at the given index
	const ofxOscBundle& getBundleAt(std::size_t i) const;

	/// \return the bundle at the given index
	ofxOscBundle& getBundleAt(std::size_t i);

	/// \return the message at the given index
	const ofxOscMessage& getMessageAt(std::size_t i) const;

	/// \return the message at the given index
	ofxOscMessage& getMessageAt(std::size_t i);

	/// output stream operator for string conversion and printing
	/// \return number of messages & bundles
	friend std::ostream& operator<<(std::ostream &os, const ofxOscBundle &sender);

private:

	std::vector<ofxOscMessage> messages; ///< bundled messages
	std::vector<ofxOscBundle> bundles; ///< bundled bundles
};
