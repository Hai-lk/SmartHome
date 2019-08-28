/**************************************************************************
   Copyright (c) 2017 sewenew

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
 *************************************************************************/

#include "sanity_test.h"
#include "utils.h"

namespace sw {

namespace redis {

namespace test {

SanityTest::SanityTest(const ConnectionOptions &opts,
                        const ConnectionOptions &cluster_opts) : _opts(opts),
                                                                    _redis(opts),
                                                                    _cluster(cluster_opts) {}

void SanityTest::run() {
    _test_uri_ctor();

    _test_move_ctor();

    _test_cmdargs();

    _test_generic_command();
}

void SanityTest::_test_uri_ctor() {
    std::string uri;
    switch (_opts.type) {
    case sw::redis::ConnectionType::TCP:
        uri = "tcp://" + _opts.host + ":" + std::to_string(_opts.port);
        break;

    case sw::redis::ConnectionType::UNIX:
        uri = "unix://" + _opts.path;
        break;

    default:
        REDIS_ASSERT(false, "Unknown connection type");
    }

    auto redis = sw::redis::Redis(uri);
    try {
        auto pong = redis.ping();
        REDIS_ASSERT(pong == "PONG", "Failed to test constructing Redis with uri");
    } catch (const sw::redis::ReplyError &e) {
        REDIS_ASSERT(e.what() == std::string("NOAUTH Authentication required."),
                "Failed to test constructing Redis with uri");
    }
}

void SanityTest::_test_move_ctor() {
    auto test_move_ctor = std::move(_redis);

    _redis = std::move(test_move_ctor);
}

void SanityTest::_test_cmdargs() {
    auto lpush_num = [](Connection &connection, const StringView &key, long long num) {
        connection.send("LPUSH %b %lld",
                        key.data(), key.size(),
                        num);
    };

    auto lpush_nums = [](Connection &connection,
                            const StringView &key,
                            const std::vector<long long> &nums) {
        CmdArgs args;
        args.append("LPUSH").append(key);
        for (auto num : nums) {
            args.append(std::to_string(num));
        }

        connection.send(args);
    };

    auto key = test_key("lpush_num");

    KeyDeleter deleter(_redis, key);

    auto reply = _redis.command(lpush_num, key, 1);
    REDIS_ASSERT(reply::parse<long long>(*reply) == 1, "failed to test cmdargs");

    std::vector<long long> nums = {2, 3, 4, 5};
    reply = _redis.command(lpush_nums, key, nums);
    REDIS_ASSERT(reply::parse<long long>(*reply) == 5, "failed to test cmdargs");

    std::vector<std::string> res;
    _redis.lrange(key, 0, -1, std::back_inserter(res));
    REDIS_ASSERT((res == std::vector<std::string>{"5", "4", "3", "2", "1"}),
            "failed to test cmdargs");
}

void SanityTest::_test_generic_command() {
    auto key = test_key("key");

    KeyDeleter deleter(_redis, key);

    std::string cmd("set");
    _redis.command(cmd, key, 123);
    auto reply = _redis.command("get", key);
    auto val = reply::parse<OptionalString>(*reply);
    REDIS_ASSERT(val && *val == "123", "failed to test generic command");

    _cluster.command(cmd, key, 456);
    reply = _cluster.command("get", key);
    val = reply::parse<OptionalString>(*reply);
    REDIS_ASSERT(val && *val == "456", "failed to test generic command");

    reply = _redis.command("incr", key);
    REDIS_ASSERT(reply::parse<long long>(*reply) == 124, "failed to test generic command");

    reply = _cluster.command("incr", key);
    REDIS_ASSERT(reply::parse<long long>(*reply) == 457, "failed to test generic command");

    _redis.command("mset", "k1", "v", "k2", "v");
    reply = _redis.command("mget", "k1", "k2");
    std::vector<OptionalString> res;
    reply::to_array(*reply, std::back_inserter(res));
    REDIS_ASSERT(res.size() == 2 && res[0] && *(res[0]) == "v" && res[1] && *(res[1]) == "v",
            "failed to test generic command");

    auto pipe = _redis.pipeline();
    auto pipe_replies = pipe.command("set", key, "value").command("get", key).exec();
    val = pipe_replies.get<OptionalString>(1);
    REDIS_ASSERT(val && *val == "value", "failed to test generic command");

    auto tx = _redis.transaction();
    auto tx_replies = tx.command("set", key, 456).command("incr", key).exec();
    REDIS_ASSERT(tx_replies.get<long long>(1) == 457, "failed to test generic command");

    auto cmd_str = {"set", key.c_str(), "new_value"};
    _redis.command(cmd_str.begin(), cmd_str.end());
    reply = _redis.command("get", key);
    val = reply::parse<OptionalString>(*reply);
    REDIS_ASSERT(val && *val == "new_value", "failed to test generic command");

    _cluster.command(cmd_str.begin(), cmd_str.end());
    reply = _cluster.command("get", key);
    val = reply::parse<OptionalString>(*reply);
    REDIS_ASSERT(val && *val == "new_value", "failed to test generic command");

    auto hash_taged_mset = {"mset", "{k}1", "v", "{k}2", "v"};
    _cluster.command(hash_taged_mset.begin(), hash_taged_mset.end());
    auto hash_taged_mget = {"mget", "{k}1", "{k}2"};
    reply = _cluster.command(hash_taged_mget.begin(), hash_taged_mget.end());
    res.clear();
    reply::to_array(*reply, std::back_inserter(res));
    REDIS_ASSERT(res.size() == 2 && res[0] && *(res[0]) == "v" && res[1] && *(res[1]) == "v",
            "failed to test generic command");
}

}

}

}
