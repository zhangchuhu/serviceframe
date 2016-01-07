#include "XmlPath.h"

static void quit(const char *fmt, ...) {
	va_list va;
	va_start(va, fmt);
	vfprintf(stderr, fmt, va);
	fprintf(stderr, "\n");
	va_end(va);
	exit(-1);
}

XmlPath XmlDocument::loadFile(const char *path, const char *top) {
	FILE *fp = fopen(path, "r");
	assert(fp != NULL);
	doc_.LoadFile(fp);
	fclose(fp);

	TiXmlElement *e = NULL;
	assert((e = doc_.FirstChildElement(top)) != NULL);
	XmlPath conf(e, top);
	assert(conf.valid());
	return conf;
}

XmlPath XmlPath::getElement(const char *path, bool required) const {
	char *tmp = NULL;
	std::string p(path);
	const char *name = strtok_r((char *)p.c_str(), "/", &tmp);
	const char *last = name;
	TiXmlElement *e = top_;

	while (1) {
		if (name == NULL) {
			break;
		}

		e = e->FirstChildElement(name);
		if (!e) {
			if (required) {
				quit("%s/%s not found", name_.c_str(), path);
			} else {
			 return XmlPath(NULL, name);
			}
		}

		last = name;
		name = strtok_r(NULL, "/", &tmp);
	}

	return XmlPath(e, last);
}

void XmlPath::next() {
	top_ = top_->NextSiblingElement(name_.c_str());
}

bool XmlPath::valid() const {
	return (top_ != NULL);
}

const char *XmlPath::getString() const {
	return top_->GetText();
}

const char *XmlPath::getString(const char *path, bool required) const {
	std::string p(path);
	char *tmp = NULL;
	const char *name = strtok_r((char *)p.c_str(), "/", &tmp);
	const char *last = name;
	const char *value = NULL;
	TiXmlElement *e = top_;

	while (1) {
		if (name == NULL) {
			break;
		}
		if (name[0] == '@') {
			last = name;
			break;
		}

		e = e->FirstChildElement(name);
		if (!e) {
			if (required) {
				quit("%s/%s not found", name_.c_str(), path);
			} else {
				return NULL;
			}
		}

		last = name;
		name = strtok_r(NULL, "/", &tmp);
	}

	if (last[0] == '@') {
		value = e->Attribute(last + 1);
	} else {
		value = e->GetText();
	}

	if (!value) {
		if (required) {
			quit("%s/%s not found", name_.c_str(), path);
		} else {
			return NULL;
		}
	}
	return value;
}

long long XmlPath::getNumber(const char *path, bool required) const {
	const char *value = getString(path, required);
	return (value != NULL ? atoll(value) : 0LL);
}

