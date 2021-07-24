Baricoin Core integration/staging tree
=====================================

http://www.fujicoin.org

What is Baricoin?
----------------

Baricoin is a fork of Bitcoin. This is the best coin intended fair distribution and to improve usability.

 - Confirmation time is ten times faster than Bitcoin, making it reliable for time-critical transactions.
 - Transaction capacity is ten times larger than original Bitcoin. You do not have to worry about transaction congestion.
 - Coins are issued based on S-curve theory, so it will realize a fair distribution over the future.
 - Baricoin is the first cryptocurrency in the world which implementted measures against double spend by 51% attack on exchange.
 - Difficulty retargeting every block to recover from large hashrate swings


For more information, as well as an immediately useable, binary version of
the Baricoin Core software, see http://www.fujicoin.org/, or read the
[What is Baricoin? page](http://www.fujicoin.org/what-is-fujicoin.php).

How to Build
------------

[GITIAN BUILD(Linux, Windows, OSX)](./doc/gitian-building.md)

[UNIX BUILD](./doc/build-unix.md)

[WINDOWS BUILD](./doc/build-windows.md)

License
-------

Baricoin Core is released under the terms of the MIT license. See [COPYING](COPYING) for more
information or see https://opensource.org/licenses/MIT.

Development Process
-------------------

The `master` branch is regularly built and tested, but is not guaranteed to be
completely stable. [Tags](https://github.com/fujicoin/fujicoin/tags) are created
regularly to indicate new official, stable release versions of Baricoin Core.

Developers work on their own forks and submit pull requests in order to merge
changes with `master`. Due to the relatively small size of the development team,
developers also commit directly to the repo often. Anyone is allowed to contribute
though and useful pull requests will almost always be accepted given various
obvious stipulations regarding stability etc. 

The Baricoin [slack](http://slack.fujicoin.org/) or [subreddit](https://reddit.com/r/fujicoin)
should be used to discuss complicated or controversial changes with the developers 
before working on a patch set.

Testing
-------

Baricoin currently relies on Bitcoin Core for its testcases, and few of them are
known to work, though the software is based on fully test conforming upstream 
Bitcoin Core versions. We would be grateful to those who can help port the existing
Bitcoin Core test cases to Baricoin such that they can be used to assure correctness.

Translations
------------

Changes to translations as well as new translations can be submitted to
[Baricoin Core's Transifex page](https://www.transifex.com/projects/p/bitcoin/).

Translations are periodically pulled from Transifex and merged into the git repository. See the
[translation process](doc/translation_process.md) for details on how this works.

**Important**: We do not accept translation changes as GitHub pull requests because the next
pull from Transifex would automatically overwrite them again.

Translators should also subscribe to the [mailing list](https://groups.google.com/forum/#!forum/bitcoin-translators).
