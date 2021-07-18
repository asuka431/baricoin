#!/usr/bin/env bash
# Copyright (c) 2016-2019 The Baricoin Core developers
# Distributed under the MIT software license, see the accompanying
# file COPYING or http://www.opensource.org/licenses/mit-license.php.

export LC_ALL=C
TOPDIR=${TOPDIR:-$(git rev-parse --show-toplevel)}
BUILDDIR=${BUILDDIR:-$TOPDIR}

BINDIR=${BINDIR:-$BUILDDIR/src}
MANDIR=${MANDIR:-$TOPDIR/doc/man}

BARICOIND=${BARICOIND:-$BINDIR/baricoind}
BARICOINCLI=${BARICOINCLI:-$BINDIR/baricoin-cli}
BARICOINTX=${BARICOINTX:-$BINDIR/baricoin-tx}
WALLET_TOOL=${WALLET_TOOL:-$BINDIR/baricoin-wallet}
BARICOINQT=${BARICOINQT:-$BINDIR/qt/baricoin-qt}

[ ! -x $BARICOIND ] && echo "$BARICOIND not found or not executable." && exit 1

# The autodetected version git tag can screw up manpage output a little bit
read -r -a BARIVER <<< "$($BARICOINCLI --version | head -n1 | awk -F'[ -]' '{ print $6, $7 }')"

# Create a footer file with copyright content.
# This gets autodetected fine for baricoind if --version-string is not set,
# but has different outcomes for baricoin-qt and baricoin-cli.
echo "[COPYRIGHT]" > footer.h2m
$BARICOIND --version | sed -n '1!p' >> footer.h2m

for cmd in $BARICOIND $BARICOINCLI $BARICOINTX $WALLET_TOOL $BARICOINQT; do
  cmdname="${cmd##*/}"
  help2man -N --version-string=${BARIVER[0]} --include=footer.h2m -o ${MANDIR}/${cmdname}.1 ${cmd}
  sed -i "s/\\\-${BARIVER[1]}//g" ${MANDIR}/${cmdname}.1
done

rm -f footer.h2m
