#ifndef GAMESERVER__NETWORK_HANDLER_HPP
#define GAMESERVER__NETWORK_HANDLER_HPP

namespace minecraft::net {

class IncomingHandler {
public:
    virtual ~IncomingHandler() = default;
    virtual void onRead(unsigned char* in, size_t length) = 0;
};

}  // namespace mincraft::net

#endif //GAMESERVER__NETWORK_HANDLER_HPP
