import {useState} from 'react';

function Header({ title }) {
	return <h1>{ title ? title: 'Default Title'}</h1>;
}

export default function Home() {
	const vals = ['foo', 'bar'];
	const [likes, setLikes] = useState(0);

	function click() { setLikes(likes + 1); }

	return (
		<div>
			<Header title="Hello World" />
			<ul>
				{ vals.map(v => <li key={v}>{v}</li>) }
			</ul>
			<button onClick={click}>{likes}</button>
		</div>
	);
}
