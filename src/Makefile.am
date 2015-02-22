DIST_SUBDIRS = secp256k1
AM_LDFLAGS = $(PTHREAD_CFLAGS) $(LIBTOOL_LDFLAGS)


if EMBEDDED_LEVELDB
LEVELDB_CPPFLAGS += -I$(srcdir)/leveldb/include
LEVELDB_CPPFLAGS += -I$(srcdir)/leveldb/helpers/memenv
LIBLEVELDB += $(builddir)/leveldb/libleveldb.a
LIBMEMENV  += $(builddir)/leveldb/libmemenv.a

# NOTE: This dependency is not strictly necessary, but without it make may try to build both in parallel, which breaks the LevelDB build system in a race
$(LIBLEVELDB): $(LIBMEMENV)

$(LIBLEVELDB) $(LIBMEMENV):
	@echo "Building LevelDB ..." && $(MAKE) -C $(@D) $(@F) CXX="$(CXX)" \
	  CC="$(CC)" PLATFORM=$(TARGET_OS) AR="$(AR)" $(LEVELDB_TARGET_FLAGS) \
          OPT="$(CXXFLAGS) $(CPPFLAGS)"
endif

VOYACOIN_CONFIG_INCLUDES=-I$(builddir)/config
VOYACOIN_INCLUDES=-I$(builddir) -I$(builddir)/obj $(BOOST_CPPFLAGS) $(LEVELDB_CPPFLAGS) $(CRYPTO_CFLAGS) $(SSL_CFLAGS)

VOYACOIN_INCLUDES += -I$(srcdir)/secp256k1/include

LIBVOYACOIN_SERVER=libvoyacoin_server.a
LIBVOYACOIN_WALLET=libvoyacoin_wallet.a
LIBVOYACOIN_COMMON=libvoyacoin_common.a
LIBVOYACOIN_CLI=libvoyacoin_cli.a
LIBVOYACOIN_UTIL=libvoyacoin_util.a
LIBVOYACOIN_CRYPTO=crypto/libvoyacoin_crypto.a
LIBVOYACOIN_UNIVALUE=univalue/libvoyacoin_univalue.a
LIBVOYACOINQT=qt/libvoyacoinqt.a
LIBSECP256K1=secp256k1/libsecp256k1.la

$(LIBSECP256K1): $(wildcard secp256k1/src/*) $(wildcard secp256k1/include/*)
	$(AM_V_at)$(MAKE) $(AM_MAKEFLAGS) -C $(@D) $(@F)

# Make is not made aware of per-object dependencies to avoid limiting building parallelization
# But to build the less dependent modules first, we manually select their order here:
EXTRA_LIBRARIES = \
  crypto/libvoyacoin_crypto.a \
  libvoyacoin_util.a \
  libvoyacoin_common.a \
  univalue/libvoyacoin_univalue.a \
  libvoyacoin_server.a \
  libvoyacoin_cli.a
if ENABLE_WALLET
VOYACOIN_INCLUDES += $(BDB_CPPFLAGS)
EXTRA_LIBRARIES += libvoyacoin_wallet.a
endif

if BUILD_VOYACOIN_LIBS
lib_LTLIBRARIES = libvoyacoinconsensus.la
LIBVOYACOIN_CONSENSUS=libvoyacoinconsensus.la
else
LIBVOYACOIN_CONSENSUS=
endif

bin_PROGRAMS =
TESTS =

if BUILD_VOYACOIND
  bin_PROGRAMS += voyacoind
endif

if BUILD_VOYACOIN_UTILS
  bin_PROGRAMS += voyacoin-cli voyacoin-tx
endif

.PHONY: FORCE
# voyacoin core #
VOYACOIN_CORE_H = \
  addrman.h \
  alert.h \
  allocators.h \
  amount.h \
  base58.h \
  bloom.h \
  chain.h \
  chainparams.h \
  chainparamsbase.h \
  chainparamsseeds.h \
  checkpoints.h \
  checkqueue.h \
  clientversion.h \
  coincontrol.h \
  coins.h \
  compat.h \
  compressor.h \
  primitives/block.h \
  primitives/transaction.h \
  core_io.h \
  crypter.h \
  db.h \
  eccryptoverify.h \
  ecwrapper.h \
  hash.h \
  init.h \
  key.h \
  keystore.h \
  leveldbwrapper.h \
  limitedmap.h \
  main.h \
  merkleblock.h \
  miner.h \
  mruset.h \
  netbase.h \
  net.h \
  noui.h \
  pow.h \
  protocol.h \
  pubkey.h \
  random.h \
  rpcclient.h \
  rpcprotocol.h \
  rpcserver.h \
  script/interpreter.h \
  script/script.h \
  script/sigcache.h \
  script/sign.h \
  script/standard.h \
  script/script_error.h \
  serialize.h \
  streams.h \
  sync.h \
  threadsafety.h \
  timedata.h \
  tinyformat.h \
  txdb.h \
  txmempool.h \
  ui_interface.h \
  uint256.h \
  undo.h \
  util.h \
  utilstrencodings.h \
  utilmoneystr.h \
  utiltime.h \
  version.h \
  wallet.h \
  wallet_ismine.h \
  compat/byteswap.h \
  walletdb.h \
  compat/endian.h \
  compat/sanity.h

JSON_H = \
  json/json_spirit.h \
  json/json_spirit_error_position.h \
  json/json_spirit_reader.h \
  json/json_spirit_reader_template.h \
  json/json_spirit_stream_reader.h \
  json/json_spirit_utils.h \
  json/json_spirit_value.h \
  json/json_spirit_writer.h \
  json/json_spirit_writer_template.h

obj/build.h: FORCE
	@$(MKDIR_P) $(builddir)/obj
	@$(top_srcdir)/share/genbuild.sh $(abs_top_builddir)/src/obj/build.h \
	  $(abs_top_srcdir)
libvoyacoin_util_a-clientversion.$(OBJEXT): obj/build.h

# server: shared between voyacoind and voyacoin-qt
libvoyacoin_server_a_CPPFLAGS = $(VOYACOIN_INCLUDES) $(MINIUPNPC_CPPFLAGS)
libvoyacoin_server_a_SOURCES = \
  addrman.cpp \
  alert.cpp \
  bloom.cpp \
  chain.cpp \
  checkpoints.cpp \
  init.cpp \
  leveldbwrapper.cpp \
  main.cpp \
  merkleblock.cpp \
  miner.cpp \
  net.cpp \
  noui.cpp \
  pow.cpp \
  rest.cpp \
  rpcblockchain.cpp \
  rpcmining.cpp \
  rpcmisc.cpp \
  rpcnet.cpp \
  rpcrawtransaction.cpp \
  rpcserver.cpp \
  script/sigcache.cpp \
  timedata.cpp \
  txdb.cpp \
  txmempool.cpp \
  $(JSON_H) \
  $(VOYACOIN_CORE_H)

# wallet: shared between voyacoind and voyacoin-qt, but only linked
# when wallet enabled
libvoyacoin_wallet_a_CPPFLAGS = $(VOYACOIN_INCLUDES)
libvoyacoin_wallet_a_SOURCES = \
  db.cpp \
  crypter.cpp \
  rpcdump.cpp \
  rpcwallet.cpp \
  wallet.cpp \
  wallet_ismine.cpp \
  walletdb.cpp \
  $(VOYACOIN_CORE_H)

# crypto primitives library
crypto_libvoyacoin_crypto_a_CPPFLAGS = $(VOYACOIN_CONFIG_INCLUDES)
crypto_libvoyacoin_crypto_a_SOURCES = \
  crypto/sha1.cpp \
  crypto/sha256.cpp \
  crypto/sha512.cpp \
  crypto/hmac_sha256.cpp \
  crypto/rfc6979_hmac_sha256.cpp \
  crypto/hmac_sha512.cpp \
  crypto/ripemd160.cpp \
  crypto/common.h \
  crypto/sha256.h \
  crypto/sha512.h \
  crypto/hmac_sha256.h \
  crypto/rfc6979_hmac_sha256.h \
  crypto/hmac_sha512.h \
  crypto/sha1.h \
  crypto/ripemd160.h

# univalue JSON library
univalue_libvoyacoin_univalue_a_SOURCES = \
  univalue/univalue.cpp \
  univalue/univalue_read.cpp \
  univalue/univalue_write.cpp \
  univalue/univalue_escapes.h \
  univalue/univalue.h

# common: shared between voyacoind, and voyacoin-qt and non-server tools
libvoyacoin_common_a_CPPFLAGS = $(VOYACOIN_INCLUDES)
libvoyacoin_common_a_SOURCES = \
  allocators.cpp \
  amount.cpp \
  base58.cpp \
  chainparams.cpp \
  coins.cpp \
  compressor.cpp \
  primitives/block.cpp \
  primitives/transaction.cpp \
  core_read.cpp \
  core_write.cpp \
  eccryptoverify.cpp \
  ecwrapper.cpp \
  hash.cpp \
  key.cpp \
  keystore.cpp \
  netbase.cpp \
  protocol.cpp \
  pubkey.cpp \
  script/interpreter.cpp \
  script/script.cpp \
  script/sign.cpp \
  script/standard.cpp \
  script/script_error.cpp \
  $(VOYACOIN_CORE_H)

# util: shared between all executables.
# This library *must* be included to make sure that the glibc
# backward-compatibility objects and their sanity checks are linked.
libvoyacoin_util_a_CPPFLAGS = $(VOYACOIN_INCLUDES)
libvoyacoin_util_a_SOURCES = \
  compat/strnlen.cpp \
  compat/glibc_sanity.cpp \
  compat/glibcxx_sanity.cpp \
  chainparamsbase.cpp \
  clientversion.cpp \
  random.cpp \
  rpcprotocol.cpp \
  sync.cpp \
  uint256.cpp \
  util.cpp \
  utilstrencodings.cpp \
  utilmoneystr.cpp \
  utiltime.cpp \
  $(VOYACOIN_CORE_H)

if GLIBC_BACK_COMPAT
libvoyacoin_util_a_SOURCES += compat/glibc_compat.cpp
libvoyacoin_util_a_SOURCES += compat/glibcxx_compat.cpp
endif

# cli: shared between voyacoin-cli and voyacoin-qt
libvoyacoin_cli_a_CPPFLAGS = $(VOYACOIN_INCLUDES)
libvoyacoin_cli_a_SOURCES = \
  rpcclient.cpp \
  $(VOYACOIN_CORE_H)

nodist_libvoyacoin_util_a_SOURCES = $(srcdir)/obj/build.h
#

# voyacoind binary #
voyacoind_LDADD = \
  $(LIBVOYACOIN_SERVER) \
  $(LIBVOYACOIN_COMMON) \
  $(LIBVOYACOIN_UNIVALUE) \
  $(LIBVOYACOIN_UTIL) \
  $(LIBVOYACOIN_CRYPTO) \
  $(LIBLEVELDB) \
  $(LIBMEMENV) \
  $(LIBSECP256K1)

if ENABLE_WALLET
voyacoind_LDADD += libvoyacoin_wallet.a
endif
voyacoind_SOURCES = voyacoind.cpp
#

if TARGET_WINDOWS
voyacoind_SOURCES += voyacoind-res.rc
endif

voyacoind_LDADD += $(BOOST_LIBS) $(BDB_LIBS) $(SSL_LIBS) $(CRYPTO_LIBS) $(MINIUPNPC_LIBS)
voyacoind_CPPFLAGS = $(VOYACOIN_INCLUDES)
voyacoind_LDFLAGS = $(RELDFLAGS) $(AM_LDFLAGS) $(LIBTOOL_APP_LDFLAGS)

# voyacoin-cli binary #
voyacoin_cli_LDADD = \
  $(LIBVOYACOIN_CLI) \
  $(LIBVOYACOIN_UTIL) \
  $(BOOST_LIBS) \
  $(SSL_LIBS) \
  $(CRYPTO_LIBS)

voyacoin_cli_SOURCES = \
  voyacoin-cli.cpp

voyacoin_cli_CPPFLAGS = $(VOYACOIN_INCLUDES)
#

# voyacoin-tx binary #
voyacoin_tx_LDADD = \
  $(LIBVOYACOIN_UNIVALUE) \
  $(LIBVOYACOIN_COMMON) \
  $(LIBVOYACOIN_UTIL) \
  $(LIBVOYACOIN_CRYPTO) \
  $(LIBSECP256K1) \
  $(BOOST_LIBS) \
  $(CRYPTO_LIBS)

voyacoin_tx_SOURCES = voyacoin-tx.cpp
voyacoin_tx_CPPFLAGS = $(VOYACOIN_INCLUDES)
#
voyacoin_tx_LDFLAGS = $(RELDFLAGS) $(AM_LDFLAGS) $(LIBTOOL_APP_LDFLAGS)

if TARGET_WINDOWS
voyacoin_cli_SOURCES += voyacoin-cli-res.rc
endif
voyacoin_cli_LDFLAGS = $(RELDFLAGS) $(AM_LDFLAGS) $(LIBTOOL_APP_LDFLAGS)

if BUILD_VOYACOIN_LIBS
include_HEADERS = script/voyacoinconsensus.h
libvoyacoinconsensus_la_SOURCES = \
  primitives/transaction.cpp \
  crypto/hmac_sha512.cpp \
  crypto/sha1.cpp \
  crypto/sha256.cpp \
  crypto/sha512.cpp \
  crypto/ripemd160.cpp \
  eccryptoverify.cpp \
  ecwrapper.cpp \
  hash.cpp \
  pubkey.cpp \
  script/script.cpp \
  script/interpreter.cpp \
  script/voyacoinconsensus.cpp \
  uint256.cpp \
  utilstrencodings.cpp

if GLIBC_BACK_COMPAT
  libvoyacoinconsensus_la_SOURCES += compat/glibc_compat.cpp
  libvoyacoinconsensus_la_SOURCES += compat/glibcxx_compat.cpp
endif

libvoyacoinconsensus_la_LDFLAGS = -no-undefined $(RELDFLAGS)
libvoyacoinconsensus_la_LIBADD = $(CRYPTO_LIBS)
libvoyacoinconsensus_la_CPPFLAGS = $(CRYPTO_CFLAGS) -I$(builddir)/obj -DBUILD_VOYACOIN_INTERNAL
if USE_LIBSECP256K1
libvoyacoinconsensus_la_LIBADD += secp256k1/libsecp256k1.la
endif
endif

CLEANFILES = leveldb/libleveldb.a leveldb/libmemenv.a *.gcda *.gcno

DISTCLEANFILES = obj/build.h

EXTRA_DIST = leveldb

clean-local:
	-$(MAKE) -C leveldb clean
	-$(MAKE) -C secp256k1 clean
	rm -f leveldb/*/*.gcno leveldb/helpers/memenv/*.gcno
	-rm -f config.h

.rc.o:
	@test -f $(WINDRES)
	$(AM_V_GEN) $(WINDRES) -DWINDRES_PREPROC -i $< -o $@

.mm.o:
	$(AM_V_CXX) $(OBJCXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) \
	  $(CPPFLAGS) $(AM_CXXFLAGS) $(QT_INCLUDES) $(CXXFLAGS)  -c -o $@ $<

%.pb.cc %.pb.h: %.proto
	@test -f $(PROTOC)
	$(AM_V_GEN) $(PROTOC) --cpp_out=$(@D) --proto_path=$(abspath $(<D) $<)

if ENABLE_TESTS
include Makefile.test.include
endif

if ENABLE_QT
include Makefile.qt.include
endif

if ENABLE_QT_TESTS
include Makefile.qttest.include
endif
