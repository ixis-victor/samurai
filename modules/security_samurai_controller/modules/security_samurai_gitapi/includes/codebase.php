<?php

	class Codebase {
		protected $account;
		protected $curl;
		protected $key;
		protected $user;

		public function __construct() {
			$this->curl = curl_init();
		}

		public function setAccount($account) {
			$this->account = $account;
		}

		public function setAuthentication($user, $key) {
			$this->key = $key;
			$this->user = $user;
		}

		public function get($path) {
			return $this->execute('GET', $path);
		}

		public function post($path, DOMDocument $document) {
			return $this->execute('POST', $path, $document->saveXML());
		}

		public function put($path, DOMDocument $document) {
			return $this->execute('PUT', $path, $document->saveXML());
		}

		protected function execute($method, $path, $data = null) {
			curl_setopt_array($this->curl, array(
				CURLOPT_URL				=> sprintf(
					'http://api3.codebasehq.com/%s', trim($path, '/')
				),
				CURLOPT_USERPWD			=> sprintf(
					'%s/%s:%s', $this->account, $this->user, $this->key
				),
				CURLOPT_RETURNTRANSFER	=> true,
				CURLOPT_HEADER			=> false,
				CURLOPT_HTTPHEADER		=> array(
					'Accept: application/xml',
					'Content-type: application/xml'
				)
			));

			if ($data !== null) {
				curl_setopt(CURLOPT_POSTFIELDS, $data);
			}

			$data = trim(curl_exec($this->curl));
			$url = curl_getinfo($this->curl, CURLINFO_EFFECTIVE_URL);
			$status = curl_getinfo($this->curl, CURLINFO_HTTP_CODE);

			if (!empty($data)) {
				$data = simplexml_load_string($data);
			}

			switch ($status) {
				case 200:
				case 201:
					return new CodebaseSuccessResponse($status, $url, $data);
				default:
					throw new CodebaseErrorResponse($status, $url, $data);
			}
		}
	}

	class CodebaseSuccessResponse {
		public $document;
		public $url;
		public $status;

		public function __construct($status, $url,  $document) {
			$this->document = $document;
			$this->url = $url;
			$this->status = $status;
		}
	}

	class CodebaseErrorResponse extends Exception {
		public $document;
		public $url;
		public $status;

		public function __construct($status, $url,  $document) {
			$this->document = $document;
			$this->url = $url;
			$this->status = $status;

			parent::__construct(sprintf("Error occured while trying to access '%s'.", $url));
		}
	}