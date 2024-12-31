# Mixedgram (work in progress)

A character-level n-gram tokenizer using mixed n sizes, for text files encoded in latin1.

## Motivation

This is a hobbyist experiment on writing (from scratch) an encoder for tokenizing a latin1 text file (which is simple to
handle). It convenietly also acts as a compression method (in best case, 16 latin1 bytes becomes 3 mixedgram bytes).

I expect to use this tokenizer for yet another future experiment: writing a tiny generative language model on my own.
It remains to be seen if I will ever finish this tokenizer right here, let alone the future not-yet-started project.

## How it builds the vocabulary

An encoded file with mixedgram (or 'tokenized' file) has a **4096 tokens long vocabulary**, in other words, a **12-bit**
vocabulary. Mixedgram works **exclusively with latin1** (extended ascii), which its exact one byte occupies 1:1 our
first 256 tokens (1 byte = 2^8 bits = 256 unique values). This leaves 3840 positions to be filled.

To select which n-grams from the source text will occupy those, and complete our vocabulary, the following is performed:

- The entire source text is broken into a set of (unique) 2-grams.
- Then 3-grams.
- Then 4, 5, 6, 7, and 8-grams.
- The differently sized n-grams are all mixed together in a single list.
- An expressivity score is computed for each n-gram entry: n-gram length times occurences in the text (n * count).
- The list is ordered descendingly according to the expressivity score (higher on top).
- The **top 3840 n-grams** are then taken.

The 8-bit encoding of latin1 plus the selected 3840 tokens from the (mixed then ordered by score) n-grams fills exactly
the 12-bit value we will dedicate for each encoded token (first 8 bits for latin1 character and 4 bits for n-grams in
the vocabulary).

Important remark: there is a bad side effect due how the algorithm above works (or maybe how simple it is). There is a
(not so small) chance to get **overlapping n-grams** on top, like having "actually", and "actuall", and "actual". Seems
redundant and wasteful.

## How text is encoded

As mentioned in the previous section, each encoded token takes exactly 12 bits (4096 unique values, 2^12).

Why 12? Because it is a good middle ground between vocabulary size (4096 unique tokens) and size on disk (1.5 bytes per
token).  1 byte gives us only 256 unique values, which is already occupied by our encoding (latin1). 2 bytes gives 65536
unique values, which is too large or a vocabulary for a tiny language model
([OpenAI's has a 200k tokens encoding](https://github.com/openai/tiktoken)). Imagine one-hot encoding 4096 vs 65536.  I
do not want to pad/waste precious 4 bits, so one byte and a half it will be!

Half a byte? Yes, each **3 bytes** (in exact triples) the encoded text represent **2 tokens**. Mixedgram encoded size in
bytes is equals to: **number of tokens \* 1.5**.

![]()

There is an unfortunate case (or two) though: since it demands exact triples, a **trailing padding byte of 0s** (or two
bytes of 0s) might be appended to make it exact as required (whole triples).

## The null character is reserved

One not-so-smooth corner of the memory layout is usage of the nulls for padding (that will be explained in a minute in
the section below). As a result, **null characters are removed from the text before processing**, which is a hard
requirement of mixedgram.

## File format

For simplicity (and library's maintenance convenience), the `.mxgram` file format stores both the vocabulary and the
encoded source text:

```
[<----- vocabulary (38400 bytes or 38.4 kb) ----->][<----- mixedgram encoded text (variable size) ----->]
```

The 3840 vocabulary entries times 10 bytes for each token gives us 38400 bytes. Here is how the 10 bytes are used:

```
[<----- latin1 encoded n-gram (8 bytes) ----->][<----- mixedgram encoded n-gram (2 bytes) ----->]
```

- We have 3840 tokens to fill (4096 - 256, that comes from 12 bits minus 1 byte of latin1).
- Each token, in which can comprise from 2 up to 8 bytes/characters of latin1 text, occupies exactly 8 bytes no matter
the text size of the token (left zeroes are added as left pad as needed).
- The next two bytes are then used to store the 12-bit encoded value (that will be used on writing the second half of
the file, the encoded text).

You may have noticed that the library is 'wasting' bytes on padding. This is by design: knowing the exact vocabulary
size no matter the case is convenient to implement. The hope is that since this tokenizer will be used on (very) large
text files, the size reduction on encoding the content (or compressing it) will easily compensate the bytes added due
padding.

## Non-configurable sizes

All mixedgram assumed sizes are hardcoded in the library, in other words, it is **not configurable** by the library
user:

Parameter                     | Value
---------                     | ------
Supported text encoding       | latin1 (256 unique characters)
N-gram range                  | 2 to 8-gram
Vocabulary size               | 4096 unique tokens (256 + 3840)
Encoded token size            | 12 bits (2^12 = 4096)
File header size (vocabulary) | 38.4 kbytes
