# Troubleshooting

Some typical errors that can happen, and how to debug and/or solve them. This is an evolving list and may differ depending on the specific version of `GeoModelXML`, as bugs are solved and others revealed.

- `evaluate: invalid expression. Last good expression was ...`:

This error indicates an issue when XML is being parsed. Check for typos or invalid syntax in defining parameters, providing names to `logvolref`, etc.