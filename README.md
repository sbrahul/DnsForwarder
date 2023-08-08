This is a simple DNS forwarder/relay that holds the original DNS request and sends back the response from the configured upstream server.

## Compilation:
A makefile is provided to compile the application. "make" command can be used to compile the executable (DnsFwder)

## Usage:
```
./DnsFwder <Upstream IPv4> <port>
```

## Example:
```bash
./DnsFwder 127.0.0.1 53
```

