import Head from 'next/head'
import Link from 'next/link'

export default function Metadata() {
    return (<>
        <Head>
            <title>Sample Title</title>
        </Head>
        <Link href="/">back</Link>
        <h1>See Title in Tab</h1>
    </>)
}