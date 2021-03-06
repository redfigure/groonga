.. -*- rst -*-

.. highlightlang:: none

.. groonga-command
.. database: commands_table_create

``table_create``
================

Summary
-------

``table_create`` creates a new table in the current database. You need
to create one or more tables to store and search data.

Syntax
------

This command takes many parameters.

The required parameter is only ``name`` and otehrs are optional::

  table_create name
               [flags=TABLE_HASH_KEY]
               [key_type=null]
               [value_type=null]
               [default_tokenizer=null]
               [normalizer=null]
               [token_filters=null]

Usage
-----

``table_create`` command creates a new persistent table. See
:doc:`/reference/tables` for table details.

Create data store table
^^^^^^^^^^^^^^^^^^^^^^^

You can use all table types for data store table. See
:doc:`/reference/tables` for all table types.

Table type is specified as ``TABLE_${TYPE}`` to ``flags`` parameter.

Here is an example to create ``TABLE_NO_KEY`` table:

.. groonga-command
.. include:: ../../example/reference/commands/table_create/data_store_table_no_key.log
.. table_create Logs TABLE_NO_KEY

The ``table_create`` command creates a table that is named ``Logs``
and is ``TABLE_NO_KEY`` type.

If your records aren't searched by key, ``TABLE_NO_KEY`` type table is
suitable. Because ``TABLE_NO_KEY`` doesn't support key but it is fast
and small table. Storing logs into Groonga database is the case.

If your records are searched by key or referenced by one or more
columns, ``TABLE_NO_KEY`` type isn't suitable. Lexicon for fulltext
search is the case.

Create lexicon table
^^^^^^^^^^^^^^^^^^^^

You can use all table types except ``TABLE_NO_KEY`` for lexicon table.
Lexicon table needs key support but ``TABLE_NO_KEY`` doesn't support
key.

Here is an example to create ``TABLE_PAT_KEY`` table:

.. groonga-command
.. include:: ../../example/reference/commands/table_create/lexicon_table_pat_key.log
.. table_create Lexicon TABLE_PAT_KEY ShortText --default_tokenizer TokenBigram --normalizer NormalizerAuto

The ``table_create`` command creates the following table:

* The table is named ``Lexicon``.
* The table is ``TABLE_PAT_KEY`` type table.
* The table's key is ``ShortText`` type.
* The table uses ``TokenBigram`` tokenizer to extract tokens from a
  normalized text.
* The table uses ``NormalizerAuto`` normalizer to normalize a text.

``TABLE_PAT_KEY`` is suitable table type for lexicon table. Lexicon
table is used for fulltext search.

In fulltext search, predictive search may be used for fuzzy
search. Predictive search is supported by ``TABLE_PAT_KEY`` and
``TABLE_DAT_KEY``.

Lexicon table has many keys because a fulltext target text has many
tokens. Table that has many keys should consider table size because
large table requires large memory. Requiring large memory causes disk
I/O. It blocks fast search. So table size is important for a table
that has many keys. ``TABLE_PAT_KEY`` is less table size than
``TABLE_DAT_KEY``.

Because of the above reasons, ``TABLE_PAT_KEY`` is suitable table type
for lexicon table.

Create tag index table
^^^^^^^^^^^^^^^^^^^^^^

You can use all table types except ``TABLE_NO_KEY`` for tag index
table. Tag index table needs key support but ``TABLE_NO_KEY`` doesn't
support key.

Here is an example to create ``TABLE_HASH_KEY`` table:

.. groonga-command
.. include:: ../../example/reference/commands/table_create/tag_index_table_hash_key.log
.. table_create Tags TABLE_HASH_KEY ShortText

The ``table_create`` command creates a table that is named ``Tags``,
is ``TABLE_HASH_KEY`` type and has ``ShortText`` type key.

``TABLE_HASH_KEY`` or ``TABLE_DAT_KEY`` are suitable table types for
tag index table.

If you need only exact match tag search feature, ``TABLE_HASH_KEY`` is
suitable. It is the common case.

If you also need predictive tag search feature (for example, searching
``"groonga"`` by ``"gr"`` keyword.), ``TABLE_DAT_KEY`` is suitable.
``TABLE_DAT_KEY`` is large table size but it is not important because
the number of tags will not be large.

Create range index table
^^^^^^^^^^^^^^^^^^^^^^^^

You can use ``TABLE_PAT_KEY`` and ``TABLE_DAT_KEY`` table types for
range index table. Range index table needs range search support but
``TABLE_NO_KEY`` and ``TABLE_HASH_KEY`` don't support it.

Here is an example to create ``TABLE_DAT_KEY`` table:

.. groonga-command
.. include:: ../../example/reference/commands/table_create/range_index_table_dat_key.log
.. table_create Ages TABLE_DAT_KEY UInt32

The ``table_create`` command creates a table that is named ``Ages``,
is ``TABLE_DAT_KEY`` type and has ``UInt32`` type key.

``TABLE_PAT_KEY`` and ``TABLE_DAT_KEY`` are suitable table types for
range index table.

If you don't have many indexed items, ``TABLE_DAT_KEY`` is
suitable. Index for age is the case in the above example. Index for
age will have only 0-100 items because human doesn't live so long.

If you have many indexed items, ``TABLE_PAT_KEY`` is suitable. Because
``TABLE_PAT_KEY`` is smaller than ``TABLE_DAT_KEY``.

Parameters
----------

This section describes all parameters.

``name``
^^^^^^^^

Specifies a table name to be created. ``name`` must be specified.

Here are available characters:

* ``0`` .. ``9`` (digit)
* ``a`` .. ``z`` (alphabet, lower case)
* ``A`` .. ``Z`` (alphabet, upper case)
* ``#`` (hash)
* ``@`` (at mark)
* ``-`` (hyphen)
* ``_`` (underscore) (NOTE: Underscore can't be used as the first
  character.)

You need to create a name with one or more the above chracters. Note
that you cannot use ``_`` as the first character such as
``_name``.

``flags``
^^^^^^^^^

Specifies a table type and table customize options.

Here are available flags:

+--------------------+------------------------------+
|      flags         | description                  |
+--------------------+------------------------------+
| ``TABLE_NO_KEY``   | Array table.                 |
+--------------------+------------------------------+
| ``TABLE_HASH_KEY`` | Hash table.                  |
+--------------------+------------------------------+
| ``TABLE_PAT_KEY``  | Patricia trie.               |
+--------------------+------------------------------+
| ``TABLE_DAT_KEY``  | Double Array trie.           |
+--------------------+------------------------------+
| ``KEY_WITH_SIS``   | Enable Semi Infinite String. |
|                    | Require ``TABLE_PAT_KEY``.   |
+--------------------+------------------------------+

.. note::
   Since Groonga 2.1.0 ``KEY_NORMALIZE`` flag is deprecated. Use
   ``normalizer`` option with ``NormalizerAuto`` instead.

You must specify one of ``TABLE_${TYPE}`` flags. You cannot specify two
or more ``TABLE_${TYPE}`` flags. For example,
``TABLE_NO_KEY|TABLE_HASH_KEY`` is invalid.

You can combine flags with ``|`` (vertical bar) such as
``TABLE_PAT_KEY|KEY_WITH_SIS``.

See :doc:`/reference/tables` for difference between table types.

The default flags are ``TABLE_HASH_KEY``.

``key_type``
^^^^^^^^^^^^

Specifies key type.

If you specify ``TABLE_HASH_KEY``, ``TABLE_PAT_KEY`` or
``TABLE_DAT_KEY`` as ``flags`` parameter, you need to specify
``key_type`` option.

See :doc:`/reference/types` for all types.

The default value is none.

``value_type``
^^^^^^^^^^^^^^

Specifies value type.

You can use value when you specify ``TABLE_NO_KEY``,
``TABLE_HASH_KEY`` or ``TABLE_PAT_KEY`` as ``flags`` parameter. Value
type must be a fixed size type. For example, ``UInt32`` can be used
but ``ShortText`` cannot be used. Use columns instead of value.

The default value is none.

``default_tokenizer``
^^^^^^^^^^^^^^^^^^^^^

Specifies the default tokenizer that is used on searching and data
loading.

You cannot use ``default_tokenizer`` with ``TABLE_NO_KEY`` because
``TABLE_NO_KEY`` cannot be used for index.

You must specify ``default_tokenizer`` for a table that is used for
fulltext search index.

See :doc:`/reference/tokenizers` for all tokenizers

The default value is none.

.. _table-create-normalizer:

``normalizer``
^^^^^^^^^^^^^^

Specifies a normalizer that is used to normalize key.

You cannot use ``normalizer`` with ``TABLE_NO_KEY`` because
``TABLE_NO_KEY`` doesn't support key.

See :doc:`/reference/normalizers` for all normalizsers.

The default value is none.

.. _table-create-token-filters:

``token_filters``
^^^^^^^^^^^^^^^^^^

Specifies token filters that is used to some processes tokenized token.

You cannot use ``token_filters`` with ``TABLE_NO_KEY`` because
``TABLE_NO_KEY`` doesn't support key.

See :doc:`/reference/token_filters` for all token filters.

The default value is none.

Return value
------------

``table_create`` returns ``true`` as body on success such as::

  [HEADER, true]

If ``table_create`` fails, error details are in ``HEADER``.

See :doc:`/reference/command/output_format` for ``HEADER``.

See also
--------

* :doc:`/reference/tables`
* :doc:`/reference/commands/column_create`
* :doc:`/reference/tokenizers`
* :doc:`/reference/normalizers`
* :doc:`/reference/command/output_format`
