// AI Token Backburn - what your AI coding tools have used, and what's left.
// Copyright (c) 2026 Daniel Rosser. MIT License.

#pragma once

#include "Usage.h"

#include <string>

namespace backburn {

// The providers' own APIs, for what the local logs can't show. Only documented endpoints are called; a key
// for a provider without one is kept but not used.
namespace providerApi {

// what the key is called and what it's for, for Settings
const char * keyName(const std::string & provider);
const char * keyUse(const std::string & provider);
bool keyUsed(const std::string & provider);       // false: stored for later, nothing calls it yet
bool needsAccount(const std::string & provider);  // xAI: a team id as well
// where the key is made, for a "Get a key" link; "" where there's no usage API to make one for
// std::string (not const char*) so Release builds can return an OBFSTR-decrypted URL — see ProviderApi.cpp.
std::string keyPage(const std::string & provider);

// Anthropic Admin API usage report (GET /v1/organizations/usage_report/messages): the Console organization's
// API use for the last 31 days by model, as a "Claude API" provider priced like Claude Code. Admin API key
// (sk-ant-admin...) from a Console organization; not available to individual or claude.ai plans.
bool anthropicUsage(const std::string & key, Provider & out, std::string & error);

// OpenAI organization usage and costs (GET /v1/organization/usage/completions and /v1/organization/costs): the
// organization's API tokens for the last 31 days by model, and what was billed each day, as an "OpenAI API"
// provider. Admin key (sk-admin-...) from the organization's settings; ChatGPT plans' Codex use isn't in it.
bool openaiUsage(const std::string & key, Provider & out, std::string & error);

// xAI Management API (GET /v1/billing/teams/{team}/prepaid/balance): the team's prepaid credit balance,
// as xAI reports it. Management key and team id from console.x.ai.
bool xaiBalance(const std::string & key, const std::string & team, std::string & balance, std::string & error);

} // namespace providerApi
} // namespace backburn
