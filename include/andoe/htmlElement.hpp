#pragma once
#include <string>
#include <vector>
#include <unordered_map>

namespace Andoe::Html {

/**
 * @brief Represents an HTML element.
 *
 * This class provides a convenient way to create and manipulate HTML elements.
 * It supports adding attributes, wrapping elements, and converting elements to strings.
 */
class HtmlElement {
private:
  std::string tag;
  std::string content;
  std::unordered_map<std::string, std::string> attributes;

public:
  /**
   * @brief Constructs an HTML element with the given text content.
   *
   * @param text The text content of the element.
   */
  explicit HtmlElement(const std::string& text);

  /**
   * @brief Constructs an HTML element with the given tag and content.
   *
   * @param tag The tag of the element.
   * @param content The content of the element.
   */
  HtmlElement(const std::string& tag, const std::string& content);

  /**
   * @brief Constructs an HTML element with the given tag and children.
   *
   * @param tag The tag of the element.
   * @param children The children of the element.
   */
  HtmlElement(const std::string& tag, const std::vector<HtmlElement>& children);

  /**
   * @brief Wraps the element with the given tag.
   *
   * @param tag The tag to wrap the element with.
   * @return The wrapped element.
   */
  HtmlElement wrap(const std::string& tag) const;

  /**
   * @brief Adds an attribute to the element.
   *
   * @param key The key of the attribute.
   * @param value The value of the attribute.
   * @return The element with the added attribute.
   */
  HtmlElement attr(const std::string& key, const std::string& value) const;

  /**
   * @brief Adds a class to the element.
   *
   * @param className The class name to add.
   * @return The element with the added class.
   */
  HtmlElement cls(const std::string& className) const;

  /**
   * @brief Adds an id to the element.
   *
   * @param idName The id name to add.
   * @return The element with the added id.
   */
  HtmlElement id(const std::string& idName) const;

  /// @name Wrapper Methods
  /// @brief Wrapper methods for common HTML elements.
  /// @{
  HtmlElement Bold() const;
  HtmlElement Italic() const;
  HtmlElement Underline() const;
  HtmlElement Strike() const;
  /// @}

  /// @name Action Methods
  /// @brief Methods for creating action elements.
  /// @{
  HtmlElement onClick(const std::string& js);
  HtmlElement action(const std::string& path, const std::string& method = "post");
  /// @}

  /**
   * @brief Converts the element to a string.
   *
   * @return The string representation of the element.
   */
  std::string str() const;
};
}
