/*
 *
 * Copyright 2015 gRPC authors.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#include "PlayerClientDB-syn.h"

Response Client::RequestDB(Request& request) {
    Response reply;

    ClientContext context;

    // The actual RPC.
    Status status = stub_->RequestDB(&context, request, &reply);

    // Act upon its status.
    if (status.ok()) {
        std::cout << "Got Reply" << std::endl;
    } else {
        std::cout << "Request Error " << status.error_code() << ": " << status.error_message() << std::endl;
    }

    return reply;
}