// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2019 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <chainparams.h>

#include <chainparamsseeds.h>
#include <consensus/merkle.h>
#include <tinyformat.h>
#include <util/system.h>
#include <util/strencodings.h>
#include <versionbitsinfo.h>

#include <assert.h>

#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>

static CBlock CreateGenesisBlock(const char* pszTimestamp, const CScript& genesisOutputScript, uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    CMutableTransaction txNew;
    txNew.nVersion = 1;
    txNew.vin.resize(1);
    txNew.vout.resize(1);
    txNew.vin[0].scriptSig = CScript() << 0 << CScriptNum(999) << std::vector<unsigned char>((const unsigned char*)pszp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
    txNew.vout[0].nValue = genesisReward;
    txNew.vout[0].scriptPubKey = genesisOutputScript;

    CBlock genesis;
    genesis.nTime    = nTime;
    genesis.nBits    = nBits;
    genesis.nNonce   = nNonce;
    genesis.nVersion = nVersion;
    genesis.vtx.push_back(MakeTransactionRef(std::move(txNew)));
    genesis.hashPrevBlock.SetNull();
    genesis.hashMerkleRoot = BlockMerkleRoot(genesis);
    return genesis;
}

/**
 * Build the genesis block. Note that the output of its generation
 * transaction cannot be spent since it did not originally exist in the
 * database.
 *
 * CBlock(hash=000000000019d6, ver=1, hashPrevBlock=00000000000000, hashMerkleRoot=4a5e1e, nTime=1231006505, nBits=1d00ffff, nNonce=2083236893, vtx=1)
 *   CTransaction(hash=4a5e1e, ver=1, vin.size=1, vout.size=1, nLockTime=0)
 *     CTxIn(COutPoint(000000, -1), coinbase 04ffff001d0104455468652054696d65732030332f4a616e2f32303039204368616e63656c6c6f72206f6e206272696e6b206f66207365636f6e64206261696c6f757420666f722062616e6b73)
 *     CTxOut(nValue=50.00000000, scriptPubKey=0x5F1DF16B2B704C8A578D0B)
 *   vMerkleTree: 4a5e1e
 */
static CBlock CreateGenesisBlock(uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    const char* pszTimestamp = "it is my birthday";
    const CScript genesisOutputScript = CScript();
    return CreateGenesisBlock(pszTimestamp, genesisOutputScript, nTime, nNonce, nBits, nVersion, genesisReward);
}

/**
 * Main network
 */
class CMainParams : public CChainParams {
public:
    CMainParams() {
        strNetworkID = CBaseChainParams::MAIN;
        consensus.nSubsidyHalvingInterval = 210000; //baricoin do not use
        consensus.BIP16Exception = uint256(); //no bip16 exception
        consensus.BIP34Height = 0;
        consensus.BIP34Hash = uint256S("わからん");
        consensus.BIP65Height = 0; // 
        consensus.BIP66Height = 0; // 
        consensus.CSVHeight = 0; // 
        consensus.SegwitHeight = 0; // 
        consensus.MinBIP9WarningHeight = 0; // segwit activation height + miner confirmation window
        consensus.powLimit = uint256S("0x3fffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetTimespan = 1.4 * 24 * 60 * 60; //1.4 days
        consensus.nPowTargetSpacing = 1.0 * 60;
        consensus.fPowAllowMinDifficultyBlocks = false;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 1512; // 75% of 2016
        consensus.nMinerConfirmationWindow = 2016; // nPowTargetTimespan / nPowTargetSpacing
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1522540800; // Apr 1st, 2018 (UTC)
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1554076800; // Apr 1st, 2019 (UTC)

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x00000000000000000000000000000000000000000000000000048095ae8a7742");

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x2aa5bda2047c8ad330319b1c78e617ba5cc51b48ee84f9dc734e8bcf8cf03c47"); // 2800000

        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 32-bit integer with any alignment.
         */
        pchMessageStart[0] = 0x66;
        pchMessageStart[1] = 0x75;
        pchMessageStart[2] = 0x6a;
        pchMessageStart[3] = 0x69;
        nDefaultPort = 8135;
        nPruneAfterHeight = 100000;
        m_assumed_blockchain_size = 6;
        m_assumed_chain_state_size = 1;

        genesis = CreateGenesisBlock(1403910000, 2560786, 0x1e0ffff0, 1, 1 * COIN); //(nTime, nNonce, nBits, nVersion, genesisReward)
        consensus.hashGenesisBlock = genesis.GetHash();
                std::cout << "Begin calculating " << strNetworkID << " Genesis Block:\n";
        arith_uint256 hashTarget = arith_uint256().SetCompact(genesis.nBits);
        uint256 hash;
        genesis.nNonce = 0;
        while (UintToArith256(genesis.GetHash()) > hashTarget) {
            ++genesis.nNonce;
            if (genesis.nNonce == 0) {
                std::cout << std::string("NONCE WRAPPED, incrementing time:\n");
                ++genesis.nTime;
            }
            if (genesis.nNonce % 1000 == 0) {
                std::cout << strNetworkID << " nonce: " << genesis.nNonce << " time: " << genesis.nTime << " hash: " << genesis.GetHash().ToString().c_str() << "\n";
            }
        }
        
        std::cout << strNetworkID << " ---\n";
        std::cout << "  nonce: " << genesis.nNonce <<  "\n";
        std::cout << "   time: " << genesis.nTime << "\n";
        std::cout << "   hash: " << genesis.GetHash().ToString().c_str() << "\n";
        std::cout << "   merklehash: "  << genesis.hashMerkleRoot.ToString().c_str() << "\n";
        std::cout << "Finished calculating " << strNetworkID << " Genesis Block:\n";
        assert(consensus.hashGenesisBlock == uint256S("0xadb6d9cfd74075e7f91608add4bd2a2ea636f70856183086842667a1597714a0"));
        assert(genesis.hashMerkleRoot == uint256S("0xf951a273c3055d1bb36b4291e7f9edd491c2d435bd5737318ef8a643cab84b61"));

        // Note that of those which support the service bits prefix, most only support a subset of
        // possible options.
        // This is fine at runtime as we'll fall back to using them as a oneshot if they don't support the
        // service bits we want, but we should get them updated to support all service bits wanted by any
        // release ASAP to avoid it where possible.
        vSeeds.emplace_back("seed1.baricoin.org"); // ?
        vSeeds.emplace_back("seed2.baricoin.org"); // Baricoin official seed

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,36);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,16);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,164);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x88, 0xB2, 0x1E};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x88, 0xAD, 0xE4};

        bech32_hrp = "bari";

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_main, pnSeed6_main + ARRAYLEN(pnSeed6_main));

        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        m_is_test_chain = false;
        m_is_mockable_chain = false;

        checkpointData = {
            {
                {      0, uint256S("0xadb6d9cfd74075e7f91608add4bd2a2ea636f70856183086842667a1597714a0")},
                { 100000, uint256S("0x1d16bb119ef3b45134055b7e16a55f99d3ef5e211922037b9fb7274a988562f6")},
                { 200000, uint256S("0x588a542ff995394bc16731e3b53ea62f403fdfec4df5f8b4a38ad61a451523ec")},
                { 300000, uint256S("0x29d9a3d44ccc482980f6e5a2559c0588dd71e9bcc802dba8c6d18d10bb406ad2")},
                { 400000, uint256S("0x3c45c562ebccdf9ee7c0cb7509d9eebcbda9298636c9f4e3950fef2f6d29b8f5")},
                { 500000, uint256S("0x91c0fbc72fc8e421d85a922ec4f6477e5763c04729d13c4f58c1b79c83232b81")},
                { 600000, uint256S("0xc7105925a54a06fb27b4574f3b5732965d7c5b975d71b91c12c1672b68045c38")},
                { 700000, uint256S("0x0e9946a0196fcd286ef16d7873b38c9729ac8c1bc6b540ec12225ebc0291d5d2")},
                { 800000, uint256S("0xf3cb5bb24c7ae26f9ffe8001d594d4a951cc9a58589ac74519357add723f5602")},
                { 900000, uint256S("0x0b5b53332af404ac740965c717a3e797cff330d9d42fb8821eedb97856d11248")},
                {1000000, uint256S("0xe267f45f1552c735a0f879403030c3f0904e8e9e0dc84cf2e6d13fcc32520dc3")},
                {1100000, uint256S("0x34c2396e76d0dba38cce0f247766e1b85fad5938598f0a4eb29c3b5aeccbb60b")},
                {1200000, uint256S("0x00d47aac9bef0bfb211ebeabf5077520da5170cb269389955d1612aa0cb2757a")},
                {1300000, uint256S("0xa4f63f7b5b92a46e73cbf3e208f5dacb2b09a39287f561541a2fc90d44b97148")},
                {1400000, uint256S("0x86c187cf79e3723189221337b457947bf88448414205e1c37435c56a9dc55052")},
                {1500000, uint256S("0xda3ce4ee4d6715aa5b9b6b94099b0d05cf28585473ab206c8b179c7a73ebbde8")},
                {1600000, uint256S("0xfbe956df17cb237f6ad438c6f94215e383259b88226ebcf38f13c611313442a7")},
                {1700000, uint256S("0xaf35ad36a43ac08d1f2d764984ccd46855d4e968853bb2287a6662ea798d6383")},
                {1800000, uint256S("0x0f5353f137b5e3f0e45d5643843ba452ac84bb6470ba608eae71994f0be87412")},
                {1900000, uint256S("0x9d95b652ba4f036bcce1f26d4832b1ee9912ec9dd7baddd156326b9b095cda79")},
                {2000000, uint256S("0xbead1047afe803c3e3b5eb246f5449e74beea3d629703c194a3fb5e78b1df6d9")},
                {2100000, uint256S("0xa878f599a700ec50b76e40af01cd746c5282f71bad7e1caba8e3d80d8a13c360")},
                {2200000, uint256S("0x3db13c96f372a19ebaeabd9943796bfcf1a096f9d89dcce0614075ae155d4a16")},
                {2300000, uint256S("0x2990b037e76a4b8415f2cdbb844e36f26142c1209b0261f8b71f7fd8ae0d6246")},
                {2400000, uint256S("0xb002e29a53e899889f6e3772386d1521d6d2301e90183ea7fec72c02f011486b")},
                {2500000, uint256S("0xab1b07c414b4e73d14bdd4b9c607e702c6c7f654c02ce647fdb44f823a208274")},
                {2600000, uint256S("0xcc108100ea1ad09b81a2cdaf879e5789f1d862a009e98704c02400bf8d0433ec")},
                {2700000, uint256S("0x266a892f2e4038fc1b2fc57fd450f1153fe9ce08fc5f8fe26c2c346b7124667e")},
                {2800000, uint256S("0x2aa5bda2047c8ad330319b1c78e617ba5cc51b48ee84f9dc734e8bcf8cf03c47")},
                {2900000, uint256S("0x26e5db2779e1ef3d4bf878950b61e6ec588334f574fb258d8cba748e2411351a")},
                {3000000, uint256S("0x6cb15b85c8753757deb55cfceadbdb99cd483ca4f65003a88b18ab0966f50949")},
            }
        };

        chainTxData = ChainTxData{
            // Data from RPC: getchaintxstats 43200 aa5f8eef49377808ce0529345dffb6cd316422bfc27c60cb7d9b2a7a008f6fbb
            /* nTime    */ 1573445634,
            /* nTxCount */ 3399778,
            /* dTxRate  */ 0.02147296260938681,
        };
    }
};

/**
 * Testnet (v3)
 */
class CTestNetParams : public CChainParams {
public:
    CTestNetParams() {
        strNetworkID = CBaseChainParams::TESTNET;
        consensus.nSubsidyHalvingInterval = 210000;
        consensus.BIP16Exception = uint256(); //no bip16 exception
        consensus.BIP34Height = 1800000;
        consensus.BIP34Hash = uint256S("0x0f5353f137b5e3f0e45d5643843ba452ac84bb6470ba608eae71994f0be87412");
        consensus.BIP65Height = 1800000; // 0x0f5353f137b5e3f0e45d5643843ba452ac84bb6470ba608eae71994f0be87412
        consensus.BIP66Height = 1800000; // 0x0f5353f137b5e3f0e45d5643843ba452ac84bb6470ba608eae71994f0be87412
        consensus.CSVHeight = 1800000; // 0x0f5353f137b5e3f0e45d5643843ba452ac84bb6470ba608eae71994f0be87412
        consensus.SegwitHeight = 1800000; // 0x0f5353f137b5e3f0e45d5643843ba452ac84bb6470ba608eae71994f0be87412
        consensus.MinBIP9WarningHeight = 1800000 + 2016; // segwit activation height + miner confirmation window
        consensus.powLimit = uint256S("00000fffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetTimespan = 1.4 * 24 * 60 * 60; 
        consensus.nPowTargetSpacing = 1.0 * 60;
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 1512; // 75% for testchains
        consensus.nMinerConfirmationWindow = 2016; // nPowTargetTimespan / nPowTargetSpacing
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1522540800; // Apr 1st, 2018 (UTC)
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1554076800; // Apr 1st, 2019 (UTC)

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x0000000000000000000000000000000000000000000000000000000000100010");

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x96bd214d68bcbfe9c786c4da26cf71fb6bbb6f24032065bdf2f4cd2b003d9c72"); // 0

        pchMessageStart[0] = 0x69;
        pchMessageStart[1] = 0x6a;
        pchMessageStart[2] = 0x75;
        pchMessageStart[3] = 0x66;
        nDefaultPort = 18135;
        nPruneAfterHeight = 1000;
        m_assumed_blockchain_size = 6;
        m_assumed_chain_state_size = 1;

        genesis = CreateGenesisBlock(1402725600, 3578955, 0x1e0ffff0, 1, 1 * COIN); //(nTime, nNonce, nBits, nVersion, genesisReward)
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0x96bd214d68bcbfe9c786c4da26cf71fb6bbb6f24032065bdf2f4cd2b003d9c72"));
        assert(genesis.hashMerkleRoot == uint256S("0xf951a273c3055d1bb36b4291e7f9edd491c2d435bd5737318ef8a643cab84b61"));

        vFixedSeeds.clear();
        vSeeds.clear();
        // nodes with support for servicebits filtering should be at the top
        //vSeeds.emplace_back("testseed.baricoin.org");

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,74);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,196);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,202);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x35, 0x87, 0xCF};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x35, 0x83, 0x94};

        bech32_hrp = "tf";

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_test, pnSeed6_test + ARRAYLEN(pnSeed6_test));

        fDefaultConsistencyChecks = false;
        fRequireStandard = false;
        m_is_test_chain = true;
        m_is_mockable_chain = false;

        checkpointData = {
            {
                {0, uint256S("0x96bd214d68bcbfe9c786c4da26cf71fb6bbb6f24032065bdf2f4cd2b003d9c72")},
            }
        };

        chainTxData = ChainTxData{
            // Data from RPC: getchaintxstats 4096 00000000000000b7ab6ce61eb6d571003fbe5fe892da4c9b740c49a07542462d
            /* nTime    */ 0,
            /* nTxCount */ 0,
            /* dTxRate  */ 0,
        };
    }
};

/**
 * Regression test
 */
class CRegTestParams : public CChainParams {
public:
    explicit CRegTestParams(const ArgsManager& args) {
        strNetworkID =  CBaseChainParams::REGTEST;
        consensus.nSubsidyHalvingInterval = 150;
        consensus.BIP16Exception = uint256(); // always enforce P2SH BIP16 on regtest
        consensus.BIP34Height = 1800000; // BIP34 has not activated on regtest (far in the future so block v1 are not rejected in tests)
        consensus.BIP34Hash = uint256S("0x0f5353f137b5e3f0e45d5643843ba452ac84bb6470ba608eae71994f0be87412");
        consensus.BIP65Height = 1800000; // BIP65 activated on regtest (Used in rpc activation tests)
        consensus.BIP66Height = 1800000; // BIP66 activated on regtest (Used in rpc activation tests)
        consensus.CSVHeight = 432; // CSV activated on regtest (Used in rpc activation tests)
        consensus.SegwitHeight = 0; // SEGWIT is always activated on regtest unless overridden
        consensus.MinBIP9WarningHeight = 0;
        consensus.powLimit = uint256S("7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetTimespan = 1.4 * 24 * 60 * 60; // 1.4 days
        consensus.nPowTargetSpacing = 1.0 * 60;
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = true;
        consensus.nRuleChangeActivationThreshold = 108; // 75% for testchains
        consensus.nMinerConfirmationWindow = 144; // Faster than normal for regtest (144 instead of 2016)
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x00");

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x00");

        pchMessageStart[0] = 0x69;
        pchMessageStart[1] = 0x6a;
        pchMessageStart[2] = 0x75;
        pchMessageStart[3] = 0x66;
        nDefaultPort = 16777;
        nPruneAfterHeight = 1000;
        m_assumed_blockchain_size = 0;
        m_assumed_chain_state_size = 0;

        UpdateActivationParametersFromArgs(args);

        genesis = CreateGenesisBlock(1402725600, 3578955, 0x1e0ffff0, 1, 1 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0x96bd214d68bcbfe9c786c4da26cf71fb6bbb6f24032065bdf2f4cd2b003d9c72"));
        assert(genesis.hashMerkleRoot == uint256S("0xf951a273c3055d1bb36b4291e7f9edd491c2d435bd5737318ef8a643cab84b61"));

        vFixedSeeds.clear(); //!< Regtest mode doesn't have any fixed seeds.
        vSeeds.clear();      //!< Regtest mode doesn't have any DNS seeds.

        fDefaultConsistencyChecks = true;
        fRequireStandard = true;
        m_is_test_chain = true;
        m_is_mockable_chain = true;

        checkpointData = {
            {
                {0, uint256S("0x96bd214d68bcbfe9c786c4da26cf71fb6bbb6f24032065bdf2f4cd2b003d9c72")},
            }
        };

        chainTxData = ChainTxData{
            0,
            0,
            0
        };

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,111);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,196);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,239);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x35, 0x87, 0xCF};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x35, 0x83, 0x94};

        bech32_hrp = "fcrt";
    }

    /**
     * Allows modifying the Version Bits regtest parameters.
     */
    void UpdateVersionBitsParameters(Consensus::DeploymentPos d, int64_t nStartTime, int64_t nTimeout)
    {
        consensus.vDeployments[d].nStartTime = nStartTime;
        consensus.vDeployments[d].nTimeout = nTimeout;
    }
    void UpdateActivationParametersFromArgs(const ArgsManager& args);
};

void CRegTestParams::UpdateActivationParametersFromArgs(const ArgsManager& args)
{
    if (gArgs.IsArgSet("-segwitheight")) {
        int64_t height = gArgs.GetArg("-segwitheight", consensus.SegwitHeight);
        if (height < -1 || height >= std::numeric_limits<int>::max()) {
            throw std::runtime_error(strprintf("Activation height %ld for segwit is out of valid range. Use -1 to disable segwit.", height));
        } else if (height == -1) {
            LogPrintf("Segwit disabled for testing\n");
            height = std::numeric_limits<int>::max();
        }
        consensus.SegwitHeight = static_cast<int>(height);
    }

    if (!args.IsArgSet("-vbparams")) return;

    for (const std::string& strDeployment : args.GetArgs("-vbparams")) {
        std::vector<std::string> vDeploymentParams;
        boost::split(vDeploymentParams, strDeployment, boost::is_any_of(":"));
        if (vDeploymentParams.size() != 3) {
            throw std::runtime_error("Version bits parameters malformed, expecting deployment:start:end");
        }
        int64_t nStartTime, nTimeout;
        if (!ParseInt64(vDeploymentParams[1], &nStartTime)) {
            throw std::runtime_error(strprintf("Invalid nStartTime (%s)", vDeploymentParams[1]));
        }
        if (!ParseInt64(vDeploymentParams[2], &nTimeout)) {
            throw std::runtime_error(strprintf("Invalid nTimeout (%s)", vDeploymentParams[2]));
        }
        bool found = false;
        for (int j=0; j < (int)Consensus::MAX_VERSION_BITS_DEPLOYMENTS; ++j) {
            if (vDeploymentParams[0] == VersionBitsDeploymentInfo[j].name) {
                UpdateVersionBitsParameters(Consensus::DeploymentPos(j), nStartTime, nTimeout);
                found = true;
                LogPrintf("Setting version bits activation parameters for %s to start=%ld, timeout=%ld\n", vDeploymentParams[0], nStartTime, nTimeout);
                break;
            }
        }
        if (!found) {
            throw std::runtime_error(strprintf("Invalid deployment (%s)", vDeploymentParams[0]));
        }
    }
}

static std::unique_ptr<const CChainParams> globalChainParams;

const CChainParams &Params() {
    assert(globalChainParams);
    return *globalChainParams;
}

std::unique_ptr<const CChainParams> CreateChainParams(const std::string& chain)
{
    if (chain == CBaseChainParams::MAIN)
        return std::unique_ptr<CChainParams>(new CMainParams());
    else if (chain == CBaseChainParams::TESTNET)
        return std::unique_ptr<CChainParams>(new CTestNetParams());
    else if (chain == CBaseChainParams::REGTEST)
        return std::unique_ptr<CChainParams>(new CRegTestParams(gArgs));
    throw std::runtime_error(strprintf("%s: Unknown chain %s.", __func__, chain));
}

void SelectParams(const std::string& network)
{
    SelectBaseParams(network);
    globalChainParams = CreateChainParams(network);
}
