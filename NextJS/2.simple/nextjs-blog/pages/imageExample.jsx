import Image from 'next/image'

export default function ImageExample() {
    return (<>
        <Image 
            src="/tree.png"
            height={144} // Desired size with correct aspect ratio
            width={144} // Desired size with correct aspect ratio
            alt="Your Name"
        />
    </>)
}