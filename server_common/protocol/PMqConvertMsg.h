#ifndef PMQCONVERTORMSG_H_
#define PMQCONVERTORMSG_H_

#include "common/packet.h"
#include "common/core/base_svid.h"
#include "common/protocol/const.h"
#include <string>
#include <map>
#include <zlib.h>

namespace server {
namespace login {

struct PMQConvertorMsg: public sox::Marshallable {
	enum {
		uri = (100 << 8 | protocol::LG_GENERIC_INTERNAL_SVID)
	};

	enum Type {
		UINT32, STRING, BOOLEAN
	};

	std::string msgName;
	std::map<std::string, Type> fieldDefs;
	std::map<std::string, uint32_t> uint32Fields;
	std::map<std::string, std::string> stringFields;
	//std::map<std::string, int32_t> int32Fields;
	std::map<std::string, bool> boolFields;

	// Reset all the contents.
	void clear() {
		msgName.clear();
		fieldDefs.clear();
		uint32Fields.clear();
		stringFields.clear();
		boolFields.clear();
	}

	bool verifySanity() {
		// We just verify that each name in fieldsDef has corret value in the specified type
		for (std::map<std::string, Type>::const_iterator i = fieldDefs.begin();
				i != fieldDefs.end(); ++i) {
			switch (i->second) {
			case UINT32:
				if (uint32Fields.find(i->first) == uint32Fields.end())
					return false;
				break;
			case STRING:
				if (stringFields.find(i->first) == stringFields.end())
					return false;
				break;
			case BOOLEAN:
				if (boolFields.find(i->first) == boolFields.end())
					return false;
				break;
			}
		}

		return true;
	}

	bool setUint32Field(std::string name, uint32_t val) {
		// check if it's already exists and if exists if that can cause conflict
		std::map<std::string, Type>::iterator i = fieldDefs.find(name);

		if (i != fieldDefs.end()) {
			Type t = i->second;
			if (t != UINT32)
				return false;
		}

		fieldDefs[name] = UINT32;
		uint32Fields[name] = val;
		return true;
	}

	bool getUint32Field(const std::string &name, uint32_t &valOut) const {
		// check if it's already exists
		std::map<std::string, uint32_t>::const_iterator i = uint32Fields.find(name);

		if (i == uint32Fields.end()) {
				return false;
		}

		valOut = uint32Fields.at(name);
		return true;
	}

	bool setStringField(std::string name, std::string val) {
		// check if it's already exists and if exists if that can cause conflict
		std::map<std::string, Type>::iterator i = fieldDefs.find(name);

		if (i != fieldDefs.end()) {
			Type t = i->second;
			if (t != STRING)                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                             
				return false;
		}

		fieldDefs[name] = STRING;
		stringFields[name] = val;
		return true;
	}

	bool getStringField(const std::string &name, std::string &valOut) const {
		// check if it's already exists
		std::map<std::string, std::string>::const_iterator i = stringFields.find(name);

		if (i == stringFields.end()) {
				return false;
		}

		valOut = stringFields.at(name);
		return true;
	}

	/* NOT SUPPORTED BY YY-FRAMEWORK
	 bool setInt32Field(std::string name, int32_t val) {
	 // check if it's already exists and if exists if that can cause conflict
	 std::map<std::string, Type>::iterator i = fieldDefs.find(name);

	 if (i != fieldDefs.end()) {
	 Type t = i->second;
	 if (t != INT32)
	 return false;
	 }

	 fieldDefs[name] = INT32;
	 int32Fields[name] = val;
	 return true;
	 }
	 */

	bool setBooleanField(std::string name, bool val) {
		// check if it's already exists and if exists if that can cause conflict
		std::map<std::string, Type>::iterator i = fieldDefs.find(name);

		if (i != fieldDefs.end()) {
			Type t = i->second;
			if (t != BOOLEAN)
				return false;
		}

		fieldDefs[name] = BOOLEAN;
		boolFields[name] = val;
		return true;
	}

	bool getBooleanField(const std::string &name, bool &valOut) const {
		// check if it's already exists
		std::map<std::string, bool>::const_iterator i = boolFields.find(name);

		if (i == boolFields.end()) {
				return false;
		}

		valOut = boolFields.at(name);
		return true;
	}

	bool getValueAsString(const std::string &name, std::string &valOut) const {
		// check if it's already exists and if exists if that can cause conflict
		std::map<std::string, Type>::const_iterator i = fieldDefs.find(name);

		if (i == fieldDefs.end()) {
			return false;
		}

		Type t = i->second;

		switch(t) {
		case UINT32:
			char buf[16];
			sprintf(buf,"%u",uint32Fields.at(name));
			valOut = buf;
			break;
		case STRING:
			valOut = stringFields.at(name);
			break;
		case BOOLEAN:
			if (boolFields.at(name)) {
				valOut = "TRUE";
			} else {
				valOut = "FALSE";
			}
			break;
		default:
			break;
		}

		return true;
	}

	virtual void marshal(sox::Pack &p) const {
		p << msgName;

		p << (uint32_t)fieldDefs.size();

		for (std::map<std::string, Type>::const_iterator i = fieldDefs.begin();
				i != fieldDefs.end(); ++i) {
			p << i->first;
			p << (uint32_t) i->second;
		}

		p << (uint32_t)uint32Fields.size();
		for (std::map<std::string, uint32_t>::const_iterator i =
				uint32Fields.begin(); i != uint32Fields.end(); ++i) {
			p << i->first;
			p << i->second;
		}

		p << (uint32_t)stringFields.size();
		for (std::map<std::string, std::string>::const_iterator i =
				stringFields.begin(); i != stringFields.end(); ++i) {
			p << i->first;
			p << i->second;
		}

//        p << int32Fields.size();
//        for (std::map<std::string, int32_t>::const_iterator i=int32Fields.begin(); i != int32Fields.end(); ++i) {
//                p << i->first;
//                p << i->second;
//        }

		p << (uint32_t)boolFields.size();
		for (std::map<std::string, bool>::const_iterator i = boolFields.begin();
				i != boolFields.end(); ++i) {
			p << i->first;
			p << i->second;
		}

	}

	virtual void unmarshal(const sox::Unpack &p) {
		p >> msgName;

		uint32_t size = 0;
		std::string name;

		p >> size;
		uint32_t typeVal;
		for (uint32_t i = 0; i < size; i++) {
			p >> name;
			p >> typeVal;

			fieldDefs[name] = (Type) typeVal;
		}

		p >> size;
		uint32_t uint32Val;
		for (uint32_t i = 0; i < size; i++) {
			p >> name;
			p >> uint32Val;

			uint32Fields[name] = uint32Val;
		}

		p >> size;
		std::string stringVal;
		for (uint32_t i = 0; i < size; i++) {
			p >> name;
			p >> stringVal;

			stringFields[name] = stringVal;
		}

		p >> size;
		bool boolVal;
		for (uint32_t i = 0; i < size; i++) {
			p >> name;
			p >> boolVal;

			boolFields[name] = boolVal;
		}

	}

};

struct PMQConvertorBatchMsg: public sox::Marshallable {
	enum {
		uri = (101 << 8 | protocol::LG_GENERIC_INTERNAL_SVID)
	};

	const static int COMPRESS_LEVEL = 3;
	const static uint32_t MIN_ZIP_SIZE = 128;

	// Don't worry. we might initialize unpacker with zipped data. Until we unzip and reset unpacker,
	// we can't use it.
	PMQConvertorBatchMsg() :
			needZip(true), isZipped(false), payloadUnzippedSize(0), msgUnpacker(payload.data(),
					payload.size()) {

	}

	void setZip(bool val) {
		needZip = val;
	}

	void add(const PMQConvertorMsg &msg) {
		// manually marshal the message and append to payload
		sox::PackBuffer buffer;
		sox::Pack pk(buffer);
		msg.marshal(pk);
		payload.append(pk.data(), pk.size());
		payloadUnzippedSize = payload.size();
	}

	/*
	 * Unzip payload. If don't need to unzip or unzip fail, return false.
	 */
	bool unzip() {
		if (isZipped) {
			Bytef *buf = new Bytef[payloadUnzippedSize + 10];
			uLong dLength = payloadUnzippedSize;

			int ret = uncompress(buf, &dLength, (const Bytef *) payload.data(),
					(uLong) payload.size());
			if (ret != Z_OK) {
				delete[] buf;
				return false;
			} else {
				isZipped = false;
				payload.assign((const char *) buf, dLength);
				delete[] buf;

				// Reload unpacker
				msgUnpacker.reset(payload.data(), payload.size());

				return true;
			}
		}

		return false;
	}

	/*
	 * Just like JDBC next, call once, return one message and move pointer to next one
	 * Return NULL when there is no more message
	 */
	PMQConvertorMsg * fetchNextMsg() {
		if (isZipped) {
			if (!unzip()) {
				log(Error, "Unzip PMQConvertorBatchMsg message fail!");
				return NULL;
			}
		}

		if (msgUnpacker.empty())
			return NULL;

		curMsg.clear();
		curMsg.unmarshal(msgUnpacker);

		return &curMsg;
	}

	virtual void marshal(sox::Pack &p) const {

		// If need zip and size is not so little, just zip it first
		if (needZip && payload.size() >= MIN_ZIP_SIZE && !isZipped) {
			uLong dlength = compressBound(payload.size());
			Bytef *buf = new Bytef[dlength];
			if (buf == NULL) {
				log(Error, "Out of memory!!");
				return;
			}
			memset(buf, 0, dlength);
			int ret = compress2(buf, &dlength, (const Bytef *) payload.data(),
					(uLong) payload.size(), COMPRESS_LEVEL);

			if (ret == Z_OK) {
				if (dlength < payload.size()) {
					isZipped = true;
					payload.assign((const char *) buf, dlength);
				} else {
					// We don't use zip since the size is bigger
					// Please add more data once
//					log(Warn,
//							"Zipped size:%d is bigger then original size:%d,won't zip. Plz add more data!",
//							dlength, payload.size());
				}
			} else {
				throw sox::UnpackError("PMQConvertorBatchMsg zip fail!");
			}
			delete[] buf;
		}

		p << needZip << isZipped << payloadUnzippedSize;

		// By default << operator marshal method, string size is limited to uint16
		// Let's use special one to handle this
		p.push_varstr32(payload.data(), payload.size());

	}

	virtual void unmarshal(const sox::Unpack &p) {
		p >> needZip >> isZipped >> payloadUnzippedSize;
		payload = p.pop_varstr32();

		//   Sometimes, producer will send empty batch msg
		/*
		if (payloadUnzippedSize == 0 || payload.size() == 0) {
			throw sox::UnpackError("Mal data! Size is zero");
		}*/

		// We don't do unzip here. Leave it to the first time when we need to get data
		msgUnpacker.reset(payload.data(), payload.size());
	}

private:
	bool needZip;
	mutable bool isZipped;
	mutable std::string payload;
	mutable uint32_t payloadUnzippedSize;

	/* Internal use. Won't marshal */
	sox::Unpack msgUnpacker;
	PMQConvertorMsg curMsg;
};

}
}

#endif
