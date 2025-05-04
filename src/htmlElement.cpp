#include "htmlElement.hpp"
#include <sstream>

namespace Andoe::Html {

  HtmlElement::HtmlElement(const std::string& text) : content(text) {}

  HtmlElement::HtmlElement(const std::string& tag, const std::string& content)
    : tag(tag), content(content) {}

  HtmlElement::HtmlElement(const std::string& tag, const std::vector<HtmlElement>& children) : tag(tag) {
    for (const auto& child : children) {
      content += child.str();
    }
  }

  HtmlElement HtmlElement::wrap(const std::string& wrapperTag) const {
    HtmlElement wrapped(*this);
    wrapped.tag = wrapperTag;
    return wrapped;
  }

  HtmlElement HtmlElement::attr(const std::string& key, const std::string& value) const {
    HtmlElement copy(*this);
    copy.attributes[key] = value;
    return copy;
  }

  HtmlElement HtmlElement::cls(const std::string& className) const {
    HtmlElement copy(*this);
    copy.attributes["class"] = className;
    return copy;
  }

  HtmlElement HtmlElement::id(const std::string& idName) const {
    HtmlElement copy(*this);
    copy.attributes["id"] = idName;
    return copy;
  }

  HtmlElement HtmlElement::Bold() const {
    return wrap("b");
  }

  HtmlElement HtmlElement::Italic() const {
    return wrap("i");
  }

  HtmlElement HtmlElement::Underline() const {
    return wrap("u");
  }

  HtmlElement HtmlElement::Strike() const {
    return wrap("s");
  }

  std::string HtmlElement::str() const {
    std::ostringstream out;

    if (!tag.empty()) {
      out << "<" << tag;
      for (const auto& [key, value] : attributes) {
        out << " " << key << "=\"" << value << "\"";
      }
      out << ">" << content << "</" << tag << ">";
    }
    else {
      out << content;
    }

    return out.str();
  }
}
