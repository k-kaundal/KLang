# example-blog-api

A KLang server application.

## Getting Started

```bash
# Install dependencies
klang pkg install

# Run in development mode (with hot reload)
klang run src/main.kl --mode=dev

# Run in production mode
klang run src/main.kl --mode=production

# Run tests
klang test tests/
```

## Project Structure

```
example-blog-api/
├── src/
│   ├── main.kl              # Application entry point
│   ├── app.kl               # Application setup
│   ├── controllers/         # Request handlers
│   ├── services/            # Business logic
│   ├── middleware/          # Custom middleware
│   ├── models/              # Data models
│   ├── routes/              # Route definitions
│   └── config/              # Configuration
├── tests/
│   ├── unit/               # Unit tests
│   └── integration/        # Integration tests
├── public/                 # Static files
└── .env                    # Environment variables
```

## Available Commands

- `klang generate module <name>` - Generate a new module
- `klang generate controller <name>` - Generate a new controller
- `klang generate service <name>` - Generate a new service
- `klang generate middleware <name>` - Generate new middleware

## API Endpoints

See the route files in `src/routes/` for available endpoints.
