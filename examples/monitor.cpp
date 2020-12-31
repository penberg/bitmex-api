//
//  Example application that uses BitMEX C++ API with the WebSocket++ library
//
//  Copyright 2021 Pekka Enberg
//
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
//  Unless required by applicable law or agreed to in writing, software
//  distributed under the License is distributed on an "AS IS" BASIS,
//  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//  See the License for the specific language governing permissions and
//  limitations under the License.
//

#include <bitmex/bitmex.hpp>

#include <websocketpp/client.hpp>
#include <websocketpp/config/asio_client.hpp>

#include <string>

using client = websocketpp::client<websocketpp::config::asio_tls_client>;
using context_ptr = websocketpp::lib::shared_ptr<websocketpp::lib::asio::ssl::context>;

using websocketpp::lib::bind;
using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;

static context_ptr on_tls_init(websocketpp::connection_hdl)
{
    using namespace boost::asio::ssl;
    context_ptr ctx = websocketpp::lib::make_shared<context>(context::sslv23);
    return ctx;
}

static void on_open(client* c, bitmex::websocket::Client* bmx_client, websocketpp::connection_hdl hdl)
{
    // Make a subscription message for "XBTUSD" symbol on the trade topic...
    auto msg = bmx_client->make_subscribe("XBTUSD", bitmex::websocket::Topic::Trade);

    /// ...and send it over the WebSocket.
    c->send(hdl, msg, websocketpp::frame::opcode::text);
}

static void on_message(bitmex::websocket::Client* client, websocketpp::connection_hdl, client::message_ptr msg)
{
    // Parse the message received from BitMEX. Note that "client" invokes all the relevant callbacks.
    client->parse_msg(msg->get_payload());
}

int main()
{
    // Create BitMEX C++ API client object.
    bitmex::websocket::Client bmx_client;

    // Register a callback that is invoked when a trade is reported.
    bmx_client.on_trade([](const char* symbol, const char* side, int size, double price) {
        std::cout << symbol << " -> " << size << " @ " << price << " (" << side << ")" << std::endl;
    });

    // Setup WebSocket++ library that connects to the BitMEX realtime feed...
    std::string uri = "wss://www.bitmex.com/realtime";
    client c;
    c.clear_access_channels(websocketpp::log::alevel::frame_payload);
#if 0
    c.set_access_channels(websocketpp::log::alevel::all);
    c.set_error_channels(websocketpp::log::elevel::all);
#endif
    c.init_asio();
    c.set_tls_init_handler(&on_tls_init);
    c.set_open_handler(bind(&on_open, &c, &bmx_client, ::_1));
    c.set_message_handler(bind(&on_message, &bmx_client, ::_1, ::_2));

    websocketpp::lib::error_code ec;
    client::connection_ptr con = c.get_connection(uri, ec);
    if (ec) {
        std::cout << "Failed to create connection: " << ec.message() << std::endl;
        return 1;
    }
    c.get_alog().write(websocketpp::log::alevel::app, "Connecting to " + uri);

    c.connect(con);

    c.run();
}
