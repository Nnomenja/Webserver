#include "ErrorProcess.hpp"
#include "../../Data/Client.hpp"
#include <string>
#include <sstream>
#include <ctime>

std::string     readErrorPageFile(std::string &path)
{
   std::ifstream file(path.c_str());
    if (!file)
       throw std::exception();

    std::ostringstream buffer;
    buffer << file.rdbuf();
    return (buffer.str());
}

/*
 * generate_error_response()
 *
 * Builds a complete HTTP/1.1 response string containing an HTML error page
 * styled after Next.js minimal dark error pages.
 *
 * Parameters:
 *   status_code    - HTTP status code (e.g. 404, 500)
 *   status_message - HTTP status message (e.g. "Not Found", "Internal Server Error")
 *
 * Returns:
 *   A std::string ready to be written directly into a socket.
 *   Format: HTTP headers + blank line + HTML body
 */
std::string generateDefaultErrorpage(int status_code, const std::string &status_message)
{
    /* ── 1. Build the HTML body ── */
    std::ostringstream html;

    html <<
        "<!DOCTYPE html>\n"
        "<html lang=\"en\">\n"
        "<head>\n"
        "  <meta charset=\"UTF-8\" />\n"
        "  <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\" />\n"
        "  <title>" << status_code << " | " << status_message << "</title>\n"
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
        "    <div class=\"code\">" << status_code << "</div>\n"
        "    <div class=\"info\">\n"
        "      <p class=\"message\">" << status_message << "</p>\n"
        "      <p class=\"sub\"><a href=\"/\">Return to home</a></p>\n"
        "    </div>\n"
        "  </div>\n"
        "  <footer>server &middot; http/1.1</footer>\n"
        "</body>\n"
        "</html>\n";

    std::string body = html.str();

    /* ── 2. Build the HTTP headers ── */
    // std::ostringstream response;

    // response
    //     << "HTTP/1.1 " << status_code << " " << status_message << "\r\n"
    //     << "Content-Type: text/html; charset=UTF-8\r\n"
    //     << "Content-Length: " << body.size() << "\r\n"
    //     << "Connection: close\r\n"
    //     << "\r\n"          /* blank line separating headers from body */
    //     << body;

    return (body);
}


/* ── Demo / manual test ─────────────────────────────────────────────────── */

void    ErrorProcess::processError(const ServerException &e, Client *client)
{
    std::string ErrorPagePath = client->getDefaultErrorPagePath(e.getCode());
    Response *res = client->getResponse();
    std::stringstream ss;

    res->setStatusCode(e.getCode());
    res->setStatusName(e.getName());
    try
    {
        if (!ErrorPagePath.size())
            throw std::exception();
        res->setBody(readErrorPageFile(ErrorPagePath));
    }
    catch(const std::exception& e)
    {
         res->setBody(generateDefaultErrorpage(res->getStatusCode(), res->getStatusMessage()));
         res->addHeader("Content-Type", "text/html; charset=UTF-8");
         ss << res->getBody().size();
         res->addHeader("Content-Length", ss.str());
         ss.str("");
         res->addHeader("Connection", "close");
    }
}
