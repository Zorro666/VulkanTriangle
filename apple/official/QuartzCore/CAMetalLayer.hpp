/*
 *
 * Copyright 2020-2021 Apple Inc.
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
 */

//-------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// QuartzCore/CAMetalLayer.hpp
//
//-------------------------------------------------------------------------------------------------------------------------------------------------------------

#pragma once

//-------------------------------------------------------------------------------------------------------------------------------------------------------------

#include "CAPrivate.hpp"

namespace CA
{
  class MetalLayer : public NS::Referencing< MetalLayer >
  {
    public:
      static MetalLayer*    layer();
  };
}

_NS_INLINE CA::MetalLayer* CA::MetalLayer::layer()
{
    return Object::sendMessage< CA::MetalLayer* >( _CA_PRIVATE_CLS( CAMetalLayer ), _CA_PRIVATE_SEL( layer ) );
}
