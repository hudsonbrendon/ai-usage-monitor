# Contributing

Thanks for your interest in ai-usage-monitor — contributions of all kinds are welcome!

## Getting set up

Follow the [Development guide](docs/DEVELOPMENT.md) to install PlatformIO Core and get everything ready to build and test locally.

## Workflow

1. Fork the repository and create a branch for your change.
2. Keep commits small and write imperative-mood messages (e.g. `Add Codex provider`, `Fix display overflow`).
3. Don't bundle unrelated changes in the same PR — one logical change per pull request makes review much easier.
4. Open a pull request against `main` when you're ready.

## Testing

- Run `pio test -e native` to execute the host unit tests and make sure they all stay green.
- Run `pio run -e ideaspark` to confirm the firmware still builds cleanly.
- Pure logic — parsing, settings, anything Arduino-independent — lives in the host-testable layer by design. If you add new pure logic, add host tests for it.
- For changes that touch hardware (display, networking, sensors), verify on a real device when you can. If you don't have the hardware, mention it in your PR and the maintainer can help validate.

## Code style & architecture

- Device-specific code belongs behind the Canvas, IBoard, and Provider seams — keep that boundary clean.
- Read [Architecture](docs/ARCHITECTURE.md) for an overview of how the layers fit together.
- If you're adding support for a new board or a new data provider, follow the guidance in [Extending](docs/EXTENDING.md).

## Opening a pull request

In your PR description, briefly explain:

- **What** changed and **why**.
- **How** you tested it (unit tests, firmware build, device test, or a note that hardware validation is needed).

That's all the maintainer needs to review efficiently.

## Code of conduct

Be kind and respectful to everyone involved. Constructive feedback, patience with newcomers, and a welcoming tone make this a better project for everyone.
