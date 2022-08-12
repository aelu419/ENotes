import { Html, Head, Main, NextScript } from 'next/document'

export default function Document() {
    return (<>
    {/* below is default document format used by Next, with added "lang" attribute to demonstrate customization */}
    <Html lang="en">
        <Head />
        <body>
            <Main />
            <NextScript />
        </body>
    </Html>\
    </>)
}