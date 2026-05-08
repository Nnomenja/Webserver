#include "Template.hpp"
#include <sstream>

std::string Template::Error(int code, std::string message)
{
    std::ostringstream html;

    html <<
        "<!DOCTYPE html>\n"
        "<html lang=\"en\">\n"
        "<head>\n"
        "  <meta charset=\"UTF-8\" />\n"
        "  <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\" />\n"
        "  <title>" << code << " | " << message<< "</title>\n"
        "  <style>\n"
        "    *, *::before, *::after { box-sizing: border-box; margin: 0; padding: 0; }\n"
        "    :root {\n"
        "      --bg: #0a0a0a; --fg: #ededed; --muted: #888888;\n"
        "      --border: #2a2a2a;\n"
        "      --font: 'Geist', 'Inter', system-ui, -apple-system, sans-serif;\n"
        "      --mono: 'Geist Mono', 'Fira Code', 'Menlo', monospace;\n"
        "    }\n"
        "    html, body { height: 100%; }\n"
        "    body {\n"
        "      background: var(--bg); color: var(--fg);\n"
        "      font-family: var(--font);\n"
        "      display: flex; flex-direction: column;\n"
        "      align-items: center; justify-content: center;\n"
        "      min-height: 100vh; padding: 2rem;\n"
        "    }\n"
        "    .wrapper {\n"
        "      display: flex; align-items: center;\n"
        "      gap: 2rem; max-width: 480px; width: 100%;\n"
        "    }\n"
        "    .code {\n"
        "      font-family: var(--mono); font-size: 1.75rem;\n"
        "      font-weight: 600; white-space: nowrap;\n"
        "      padding-right: 2rem;\n"
        "      border-right: 1px solid var(--border); line-height: 1;\n"
        "    }\n"
        "    .info { display: flex; flex-direction: column; gap: 0.25rem; }\n"
        "    .message { font-size: 0.9rem; font-weight: 400; }\n"
        "    .sub { font-size: 0.78rem; color: var(--muted); }\n"
        "    a { color: var(--fg); text-decoration: underline; text-underline-offset: 3px; }\n"
        "    a:hover { color: var(--muted); }\n"
        "    footer {\n"
        "      position: fixed; bottom: 1.5rem;\n"
        "      font-size: 0.72rem; color: var(--muted);\n"
        "      font-family: var(--mono); letter-spacing: 0.03em;\n"
        "    }\n"
        "  </style>\n"
        "</head>\n"
        "<body>\n"
        "  <div class=\"wrapper\">\n"
        "    <div class=\"code\">" << code << "</div>\n"
        "    <div class=\"info\">\n"
        "      <p class=\"message\">" << message << "</p>\n"
        "      <p class=\"sub\"><a href=\"/\">Return to home</a></p>\n"
        "    </div>\n"
        "  </div>\n"
        "  <footer>server &middot; http/1.1</footer>\n"
        "</body>\n"
        "</html>\n";

    return (html.str());
}



static std::string htmlEscape(const std::string& s)
{
    std::string out;
    out.reserve(s.size());
    for (std::size_t i = 0; i < s.size(); ++i) {
        switch (s[i]) {
            case '&':  out += "&amp;";  break;
            case '<':  out += "&lt;";   break;
            case '>':  out += "&gt;";   break;
            case '"':  out += "&quot;"; break;
            case '\'': out += "&#39;";  break;
            default:   out += s[i];     break;
        }
    }
    return out;
}

static std::string intToStr(std::size_t n)
{
    std::ostringstream oss;
    oss << n;
    return oss.str();
}


static std::string iconFolder()
{
    return
        "<svg width=\"18\" height=\"18\" viewBox=\"0 0 24 24\" fill=\"none\""
        " xmlns=\"http://www.w3.org/2000/svg\">"
        "<path d=\"M3 7C3 5.9 3.9 5 5 5h4.17a2 2 0 0 1 1.42.59L12 7h7"
        "c1.1 0 2 .9 2 2v8c0 1.1-.9 2-2 2H5a2 2 0 0 1-2-2V7z\""
        " fill=\"#e6a817\"/>"
        "</svg>";
}

static std::string iconFile()
{
    return
        "<svg width=\"18\" height=\"18\" viewBox=\"0 0 24 24\" fill=\"none\""
        " xmlns=\"http://www.w3.org/2000/svg\">"
        "<path d=\"M6 2h8l6 6v14a2 2 0 0 1-2 2H6a2 2 0 0 1-2-2V4"
        "a2 2 0 0 1 2-2z\" fill=\"#4a90d9\"/>"
        "<path d=\"M14 2v6h6\" fill=\"#2c5f8a\"/>"
        "<path d=\"M8 13h8M8 17h5\" stroke=\"#fff\" stroke-width=\"1.4\""
        " stroke-linecap=\"round\"/>"
        "</svg>";
}

static std::string iconDisabled()
{
    return
        "<svg width=\"18\" height=\"18\" viewBox=\"0 0 24 24\" fill=\"none\""
        " xmlns=\"http://www.w3.org/2000/svg\">"
        "<circle cx=\"12\" cy=\"12\" r=\"9\" fill=\"#444\"/>"
        "<path d=\"M7 7l10 10M17 7L7 17\" stroke=\"#666\""
        " stroke-width=\"2\" stroke-linecap=\"round\"/>"
        "</svg>";
}

static std::string iconParent()
{
    return
        "<svg width=\"18\" height=\"18\" viewBox=\"0 0 24 24\" fill=\"none\""
        " xmlns=\"http://www.w3.org/2000/svg\">"
        "<path d=\"M20 12H4M10 6l-6 6 6 6\" stroke=\"#888\""
        " stroke-width=\"2\" stroke-linecap=\"round\" stroke-linejoin=\"round\"/>"
        "</svg>";
}

static std::string buildRow(const t_dirEntry& e)
{
    bool        disabled    = e.path.empty();
    std::string safeName    = htmlEscape(e.name);
    std::string safePath    = htmlEscape(e.path);
    std::string displayName = safeName + (e.isDirectory ? "/" : "");
    std::string icon        = disabled      ? iconDisabled()
                            : e.isDirectory ? iconFolder()
                            :                 iconFile();
    if (e.name == "..")
        icon = iconParent();
    std::string nameCell    = disabled
        ? "<span class=\"disabled\">" + displayName + "</span>"
        : "<a href=\"" + safePath + "\" class=\""
          + std::string(e.isDirectory ? "dir" : "file")
          + "\">" + displayName + "</a>";

    std::string rowClass = disabled ? " class=\"row--disabled\"" : "";

    return
        "      <tr" + rowClass + ">\n"
        "        <td class=\"td-icon\">" + icon     + "</td>\n"
        "        <td>"                   + nameCell + "</td>\n"
        "      </tr>\n";
}

std::string Template::DirectoryListing(const std::vector<t_dirEntry> &entries)
{
    std::stringstream body;

    body <<
        "<!DOCTYPE html>\n"
        "<html lang=\"en\">\n"
        "<head>\n"
        "  <meta charset=\"utf-8\">\n"
        "  <title>Directory listing</title>\n"
        "  <style>\n"
        "    *, *::before, *::after { box-sizing: border-box; margin: 0; padding: 0; }\n"
        "    body   { font-family: 'Courier New', Courier, monospace;"
                      " background: #1a1a2e; color: #e0e0e0; padding: 32px 24px; }\n"
        "    h1     { font-family: Arial, sans-serif; font-size: 17px; font-weight: 600;"
                      " color: #fff; margin-bottom: 20px; padding-bottom: 12px;"
                      " border-bottom: 1px solid #2a2a4a; }\n"
        "    table  { border-collapse: collapse; }\n"
        "    td     { padding: 6px 12px; vertical-align: middle; }\n"
        "    tr:hover td          { background: #16213e; }\n"
        "    .row--disabled td    { opacity: .4; pointer-events: none; }\n"
        "    .td-icon             { width: 32px; text-align: center; }\n"
        "    a                    { text-decoration: none; }\n"
        "    a.dir                { color: #e6a817; }\n"
        "    a.file               { color: #4a90d9; }\n"
        "    a:hover              { text-decoration: underline; }\n"
        "    .disabled            { color: #555; font-style: italic; }\n"
        "    footer               { margin-top: 20px; font-size: 11px; color: #444;"
                                    " font-family: Arial, sans-serif; }\n"
        "  </style>\n"
        "</head>\n"
        "<body>\n"
        "  <h1>Directory listing</h1>\n"
        "  <table>\n"
        "    <tbody>\n";

    for (std::size_t i = 0; i < entries.size(); ++i)
        body << buildRow(entries[i]);

    body <<
        "    </tbody>\n"
        "  </table>\n"
        "  <footer>" + intToStr(entries.size()) + " entr"
        + (entries.size() == 1 ? "y" : "ies") + "</footer>\n"
        "</body>\n"
        "</html>\n";

    return (body.str());
}