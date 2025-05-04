#pragma once
#include <string>
#include <vector>
#include "htmlElement.hpp"

namespace Andoe::Html {
  // === Document Metadata ===
  /** @brief Creates an <html> element. */
  inline HtmlElement Html(const std::vector<HtmlElement>& children) { return HtmlElement("html", children); }
  /** @brief Creates a <head> element. */
  inline HtmlElement Head(const std::vector<HtmlElement>& children) { return HtmlElement("head", children); }
  /** @brief Creates a <title> element. */
  inline HtmlElement Title(const std::string& text) { return HtmlElement("title", text); }
  /** @brief Creates a <base> element with href. */
  inline HtmlElement Base(const std::string& href) { return HtmlElement("base", {{"href", href}}); }
  /** @brief Creates a <link> self-closing element. */
  inline HtmlElement Link() { return HtmlElement("link", ""); }
  /** @brief Creates a <meta> self-closing element. */
  inline HtmlElement Meta() { return HtmlElement("meta", ""); }
  /** @brief Creates a <style> element. */
  inline HtmlElement Style(const std::string& css) { return HtmlElement("style", css); }

  // === Sectioning ===
  inline HtmlElement Body(const std::vector<HtmlElement>& children) { return HtmlElement("body", children); }
  inline HtmlElement Header(const std::vector<HtmlElement>& children) { return HtmlElement("header", children); }
  inline HtmlElement Main(const std::vector<HtmlElement>& children) { return HtmlElement("main", children); }
  inline HtmlElement Section(const std::vector<HtmlElement>& children) { return HtmlElement("section", children); }
  inline HtmlElement Article(const std::vector<HtmlElement>& children) { return HtmlElement("article", children); }
  inline HtmlElement Aside(const std::vector<HtmlElement>& children) { return HtmlElement("aside", children); }
  inline HtmlElement Footer(const std::vector<HtmlElement>& children) { return HtmlElement("footer", children); }
  inline HtmlElement Address(const std::string& text) { return HtmlElement("address", text); }

  // === Headings ===
  inline HtmlElement H1(const std::string& text) { return HtmlElement("h1", text); }
  inline HtmlElement H2(const std::string& text) { return HtmlElement("h2", text); }
  inline HtmlElement H3(const std::string& text) { return HtmlElement("h3", text); }
  inline HtmlElement H4(const std::string& text) { return HtmlElement("h4", text); }
  inline HtmlElement H5(const std::string& text) { return HtmlElement("h5", text); }
  inline HtmlElement H6(const std::string& text) { return HtmlElement("h6", text); }

  // === Content Grouping ===
  inline HtmlElement P(const std::string& text) { return HtmlElement("p", text); }
  inline HtmlElement Hr() { return HtmlElement("hr", ""); }
  inline HtmlElement Pre(const std::string& text) { return HtmlElement("pre", text); }
  inline HtmlElement Blockquote(const std::string& text) { return HtmlElement("blockquote", text); }
  inline HtmlElement Ol(const std::vector<HtmlElement>& children) { return HtmlElement("ol", children); }
  inline HtmlElement Ul(const std::vector<HtmlElement>& children) { return HtmlElement("ul", children); }
  inline HtmlElement Li(const std::string& text) { return HtmlElement("li", text); }
  inline HtmlElement Dl(const std::vector<HtmlElement>& children) { return HtmlElement("dl", children); }
  inline HtmlElement Dt(const std::string& text) { return HtmlElement("dt", text); }
  inline HtmlElement Dd(const std::string& text) { return HtmlElement("dd", text); }
  inline HtmlElement Figure(const std::vector<HtmlElement>& children) { return HtmlElement("figure", children); }
  inline HtmlElement Figcaption(const std::string& text) { return HtmlElement("figcaption", text); }
  inline HtmlElement Div(const std::vector<HtmlElement>& children) { return HtmlElement("div", children); }

  // === Text Content ===
  inline HtmlElement A(const std::string& text) { return HtmlElement("a", text); }
  inline HtmlElement Em(const std::string& text) { return HtmlElement("em", text); }
  inline HtmlElement Strong(const std::string& text) { return HtmlElement("strong", text); }
  inline HtmlElement Small(const std::string& text) { return HtmlElement("small", text); }
  inline HtmlElement S(const std::string& text) { return HtmlElement("s", text); }
  inline HtmlElement Cite(const std::string& text) { return HtmlElement("cite", text); }
  inline HtmlElement Q(const std::string& text) { return HtmlElement("q", text); }
  inline HtmlElement Dfn(const std::string& text) { return HtmlElement("dfn", text); }
  inline HtmlElement Abbr(const std::string& text) { return HtmlElement("abbr", text); }
  inline HtmlElement Time(const std::string& text) { return HtmlElement("time", text); }
  inline HtmlElement Code(const std::string& text) { return HtmlElement("code", text); }
  inline HtmlElement Var(const std::string& text) { return HtmlElement("var", text); }
  inline HtmlElement Samp(const std::string& text) { return HtmlElement("samp", text); }
  inline HtmlElement Kbd(const std::string& text) { return HtmlElement("kbd", text); }
  inline HtmlElement Sub(const std::string& text) { return HtmlElement("sub", text); }
  inline HtmlElement Sup(const std::string& text) { return HtmlElement("sup", text); }
  inline HtmlElement I(const std::string& text) { return HtmlElement("i", text); }
  inline HtmlElement B(const std::string& text) { return HtmlElement("b", text); }
  inline HtmlElement U(const std::string& text) { return HtmlElement("u", text); }
  inline HtmlElement Mark(const std::string& text) { return HtmlElement("mark", text); }
  inline HtmlElement Ruby(const std::string& text) { return HtmlElement("ruby", text); }
  inline HtmlElement Rt(const std::string& text) { return HtmlElement("rt", text); }
  inline HtmlElement Rp(const std::string& text) { return HtmlElement("rp", text); }
  inline HtmlElement Bdi(const std::string& text) { return HtmlElement("bdi", text); }
  inline HtmlElement Bdo(const std::string& text) { return HtmlElement("bdo", text); }
  inline HtmlElement Span(const std::string& text) { return HtmlElement("span", text); }
  inline HtmlElement Br() { return HtmlElement("br", ""); }
  inline HtmlElement Wbr() { return HtmlElement("wbr", ""); }

  // === Media ===
  inline HtmlElement Img(const std::string& src, const std::string& alt = "") {
    HtmlElement img = HtmlElement("img", "");
    img.attr("src", src).attr("alt", alt);
    return img;
  }
  inline HtmlElement Video(const std::string& src) {
    return HtmlElement("video", { HtmlElement("source", {{"src", src}}) });
  }
  inline HtmlElement Audio(const std::string& src) {
    return HtmlElement("audio", { HtmlElement("source", {{"src", src}}) });
  }
  inline HtmlElement Source(const std::string& src, const std::string& type = "") {
    HtmlElement source = HtmlElement("source", "");
    source.attr("src", src).attr("type", type);
    return source;
  }
  inline HtmlElement Iframe(const std::string& src) {
    return HtmlElement("iframe", {{"src", src}});
  }

  // === Forms and Input Controls ===
  inline HtmlElement Form(const std::vector<HtmlElement>& children) { return HtmlElement("form", children); }
  inline HtmlElement Input(const std::string& type = "text") {
    return HtmlElement("input", {{"type", type}});
  }
  inline HtmlElement Textarea(const std::string& text = "") {
    return HtmlElement("textarea", text);
  }
  inline HtmlElement Label(const std::string& text) {
    return HtmlElement("label", text);
  }
  inline HtmlElement Select(const std::vector<HtmlElement>& options) {
    return HtmlElement("select", options);
  }
  inline HtmlElement Option(const std::string& text, const std::string& value = "") {
    HtmlElement option = HtmlElement("option", text);
    if (!value.empty()) {
      option.attr("value", value);
    }
    return option;
  }
  inline HtmlElement Fieldset(const std::vector<HtmlElement>& children) {
    return HtmlElement("fieldset", children);
  }
  inline HtmlElement Legend(const std::string& text) {
    return HtmlElement("legend", text);
  }

  // === Links and Navigation ===
  inline HtmlElement A(const std::string& href, const std::string& text) {
    HtmlElement a = HtmlElement("a", text);
    a.attr("href", href);
    return a;
  }
  inline HtmlElement Nav(const std::vector<HtmlElement>& children) {
    return HtmlElement("nav", children);
  }

  // === Advanced Formatting and Layout ===
  inline HtmlElement Table(const std::vector<HtmlElement>& rows) {
    return HtmlElement("table", rows);
  }
  inline HtmlElement Thead(const std::vector<HtmlElement>& rows) {
    return HtmlElement("thead", rows);
  }
  inline HtmlElement Tbody(const std::vector<HtmlElement>& rows) {
    return HtmlElement("tbody", rows);
  }
  inline HtmlElement Tr(const std::vector<HtmlElement>& cells) {
    return HtmlElement("tr", cells);
  }
  inline HtmlElement Th(const std::string& text) {
    return HtmlElement("th", text);
  }
  inline HtmlElement Td(const std::string& text) {
    return HtmlElement("td", text);
  }

  // === Interactive Elements ===
  inline HtmlElement Button(const std::string& text) {
    return HtmlElement("button", text);
  }
  inline HtmlElement Dropdown(const std::vector<HtmlElement>& options) {
    return HtmlElement("select", options);
  }
  inline HtmlElement SubmitButton(const std::string& text) {
    return HtmlElement("button", text).attr("type", "submit");
  }
  inline HtmlElement ResetButton(const std::string& text) {
    return HtmlElement("button", text).attr("type", "reset");
  }

  // === JavaScript Integration ===
  inline HtmlElement Script(const std::string& src) {
    return HtmlElement("script", "").attr("src", src);
  }
  inline HtmlElement InlineScript(const std::string& script) {
    return HtmlElement("script", script);
  }
  inline HtmlElement OnClick(const std::string& jsFunction) {
    return HtmlElement("button", "Click Me").attr("onclick", jsFunction);
  }

  // === Accessibility ===
  inline HtmlElement AriaLabel(const std::string& label) {
    return HtmlElement("div", "").attr("aria-label", label);
  }
  inline HtmlElement AltText(const std::string& alt) {
    HtmlElement img = HtmlElement("img", "");
    img.attr("alt", alt);
    return img;
  }
}
